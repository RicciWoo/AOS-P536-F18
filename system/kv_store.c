#include <xinu.h>
#include <kv.h>
#include <xmalloc.h>


// the hash function
int hashFunc(char *key) {
	if (key == NULL || *key == '\0') {
		return -1;
	}

	int i, hashCode = 0;
	for (i = 0; i < MAX_KEY_SIZE; i++) {
		if (*key == '\0') {
			break;
		}

		hashCode = (hashCode * 31 + *key) % HASH_TAB_SIZE;
		key++;
	}

	return hashCode;
}

// kv store initialization
int kv_init() {
	// initialize segregated memory allocation
	xmalloc_init();

	// initialize the LRU counter
	counterLRU = 0;
	totalHits = 0;            /* Total number of successful get requests */
	totalAccesses = 0;        /* Total number of get requests (including cache misses) */
	totalSetSuccess = 0;      /* Total number of successful set requests */
    // LRU_CACHE_SIZE         /* Total memory footprint of your key-value store */
    // counterLRU             /* Number of keys stored in the cache */
	totalEvictions = 0;       /* Number of items evicted */

	// initialize head and tail pointer in LRU
	headLRU = (LRUEntry_t *)getmem(sizeof(LRUEntry_t));
	if (headLRU == NULL) {
		printf("error allocating memory for headLRU, size: %d\n", sizeof(LRUEntry_t));
		return 1;
	}
	tailLRU = headLRU;

	// initialize hash table
	int i;
	for (i = 0; i < HASH_TAB_SIZE; i++) {
		// initialize the hash table of KVNode
		KVNode_t *kvNode = (KVNode_t *)getmem(sizeof(KVNode_t));
		if (kvNode == NULL) {
			printf("error allocating memory for kvNode, size: %d\n", sizeof(KVNode_t));
			return 1;
		}

		// initialize the head kvNode
		kvNode->key = NULL;
		kvNode->val = NULL;
		kvNode->next = NULL;
		kvHashTab[i] = kvNode;

		// initialize the hash table for LRUNode
		LRUNode_t *lruNode = (LRUNode_t *)getmem(sizeof(LRUNode_t));
		if (lruNode == NULL) {
			printf("error allocating memory for lruNode, size: %d\n", sizeof(LRUNode_t));
			return 1;
		}

		// initialize the head lruNode
		lruNode->key = NULL;
		lruNode->prev = NULL;
		lruNode->next = NULL;
		lruHashTab[i] = lruNode;
	}

	return 0;
}

/* ================= functions for KV Hash Table ================= */

// function for creating kvNode
KVNode_t *createKVNode(char *key, char *val) {
	// get length of key and value
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end
	int valLen = strlen(val) + 1; // +1 to hold '\0' at the end

	// allocation memory for key and value
	char *keyAlloc = (char *)getmem(keyLen);
	if (keyAlloc == NULL) {
		printf("error allocating memory for the key, size: %d\n", keyLen);
		return NULL;
	}
	char *valAlloc = (char *)xmalloc(valLen);
	if (valAlloc == NULL) {
		printf("error allocating memory for the value, size: %d\n", valLen);
		return NULL;
	}

	// initialize memrory for key and value
	memset((void *)keyAlloc, 0, keyLen);
	memset((void *)valAlloc, 0, valLen);

	// save key and value to the memory
	strncpy(keyAlloc, key, keyLen);
	strncpy(valAlloc, val, valLen);

	// allocate momory for kv node
	KVNode_t *kvNode = (KVNode_t *)getmem(sizeof(KVNode_t));
	if (kvNode == NULL) {
		printf("error allocating memory for kvNode, size: %d\n", sizeof(KVNode_t));
		return NULL;
	}

	// set allocated kv node
	kvNode->key = keyAlloc;
	kvNode->val = valAlloc;
	kvNode->next = NULL;

	return kvNode;
}

// function for updating value of kvNode
int updateKVNode(KVNode_t *kvNode, char *val) {
	// get length of the value
	int valLen = strlen(val) + 1; // +1 to hold '\0' at the end

	// allocation memory for the value
	char *valAlloc = (char *)xmalloc(valLen);
	if (valAlloc == NULL) {
		printf("error allocating memory for the value, size: %d\n", valLen);
		return 1;
	}

	// initialize memrory for the value
	memset((void *)valAlloc, 0, valLen);

	// save the value to the memory
	strncpy(valAlloc, val, valLen);

	// save the allocated value the the kvNode
	kvNode->val = valAlloc;

	return 0;
}

// get kvNode with the key from KV Hash Table
KVNode_t *getKVHashTab(char *key) {
	// calculat hash code of the key
	int hashCode = hashFunc(key);

	// get the head of the kv linked list
	KVNode_t *kvHead = kvHashTab[hashCode];

	// get length of key
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

	// check if the key exists
	while (kvHead->next != NULL) {
		// traverse to the next node
		kvHead = kvHead->next;

		// get the key from current node
		char *keyCurr = kvHead->key;

		// check if the keys are the same
		if (strncmp(keyCurr, key, keyLen) == 0) {
			return kvHead;
		}
	}

	// the key is not exist
	return NULL;
}

// insert kvNode into hash table, if exists overwrite
KVNode_t *setKVHashTab(char *key, char *val) {
	// check if the key exists
	KVNode_t *kvNode = getKVHashTab(key);

	// the key already exists, update the value
	if (kvNode != NULL) {
		// get the old value
		char *valOld = kvNode->val;

		// free memory of old value
		xfree((void *)valOld);

		// update the key with new value;
		updateKVNode(kvNode, val);

		return kvNode;
	}

	// create kvNode
	kvNode = createKVNode(key, val);

	// calculat hash code of the key
	int hashCode = hashFunc(key);

	// get the head of the kv linked list
	KVNode_t *kvHead = kvHashTab[hashCode];

	// traverse to the end of the linked list
	while (kvHead->next != NULL) {
		// traverse to the next node
		kvHead = kvHead->next;
	}

	// insert the kvNode at the end
	kvHead->next = kvNode;

	return kvNode;
}

// delete kvNode from hash table
int delKVHashTab(char *key) {
	// get the length of the key
	int keyLen = strlen(key) + 1;

	// calculat hash code of the key
	int hashCode = hashFunc(key);

	// get the head of the kv linked list
	KVNode_t *kvHead = kvHashTab[hashCode];

	// traverse to the end of the linked list
	while (kvHead->next != NULL) {
		// get the key from the current node
		char *keyCurr = kvHead->next->key;

		// check the keys are the same
		if (strncmp(keyCurr, key, keyLen) == 0) {
			// get the value with the key
			char *val = kvHead->next->val;

			// free memory of the value
			xfree((void *)val);
			
			// save the address for deletion
			KVNode_t *temp = kvHead->next;

			// unlink the node
			kvHead->next = kvHead->next->next;

			// free memory of the node
			freemem((char *)temp, sizeof(KVNode_t));

			// free memory of the key
			freemem(key, keyLen);

			return 0;
		}

		// traverse to the next node
		kvHead = kvHead->next;
	}

	return 1;
}

/* ================= functions for LRU Hash Table ================= */

// function for creating lruNode
LRUNode_t *createLRUNode(char *key, LRUEntry_t *prev) {
	// allocate momory for kv node
	LRUNode_t *lruNode = (LRUNode_t *)getmem(sizeof(LRUNode_t));
	if (lruNode == NULL) {
		printf("error allocating memory for lruNode, size: %d\n", sizeof(LRUNode_t));
		return NULL;
	}

	// set allocated lru node
	lruNode->key = key;
	lruNode->prev = prev;
	lruNode->next = NULL;

	return lruNode;
}

// get lruNode with the key from LRU Hash Table
LRUNode_t *getLRUHashTab(char *key) {
	// calculat hash code of the key
	int hashCode = hashFunc(key);

	// get the head of the kv linked list
	LRUNode_t *lruHead = lruHashTab[hashCode];

	// get length of key
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

	// check if the key exists
	while (lruHead->next != NULL) {
		// traverse to the next node
		lruHead = lruHead->next;

		// get the key from current node
		char *keyCurr = lruHead->key;

		// check if the keys are the same
		if (strncmp(keyCurr, key, keyLen) == 0) {
			return lruHead;
		}
	}

	// the key is not exist
	return NULL;
}

// insert lruNode into hash table, if exists overwrite
int setLRUHashTab(char *key, LRUEntry_t *prev) {
	// check if the key exists
	LRUNode_t *lruNode = getLRUHashTab(key);

	// the key already exists, update the prev pointer
	if (lruNode != NULL) {
		// update the prev pointer
		lruNode->prev = prev;

		return 0;
	}

	// create lruNode
	lruNode = createLRUNode(key, prev);

	// calculat hash code of the key
	int hashCode = hashFunc(key);

	// get the head of the lru linked list
	LRUNode_t *lruHead = lruHashTab[hashCode];

	// traverse to the end of the linked list
	while (lruHead->next != NULL) {
		// traverse to the next node
		lruHead = lruHead->next;
	}

	// insert the kvNode at the end
	lruHead->next = lruNode;

	return 0;
}

// delete lruNode from hash table
int delLRUHashTab(char *key) {
	// get the length of the key
	int keyLen = strlen(key) + 1;

	// calculat hash code of the key
	int hashCode = hashFunc(key);

	// get the head of the kv linked list
	LRUNode_t *lruHead = lruHashTab[hashCode];

	// traverse to the end of the linked list
	while (lruHead->next != NULL) {
		// get the key from the current node
		char *keyCurr = lruHead->next->key;

		// check the keys are the same
		if (strncmp(keyCurr, key, keyLen) == 0) {
			// save the address for deletion
			LRUNode_t *temp = lruHead->next;

			// unlink the node
			lruHead->next = lruHead->next->next;

			// free memory of the node
			freemem((char *)temp, sizeof(LRUNode_t));

			return 0;
		}

		// traverse to the next node
		lruHead = lruHead->next;
	}

	return 1;
}

/* ================= functions for LRU Cache ================= */

LRUEntry_t *createLRUEntry(char *key, char *val) {
	// allocate momory for LRU Entry
	LRUEntry_t *lruEntry = (LRUEntry_t *)getmem(sizeof(LRUEntry_t));
	if (lruEntry == NULL) {
		printf("error allocating memory for lruEntry, size: %d\n", sizeof(LRUEntry_t));
		return NULL;
	}

	// set allocated kv node
	lruEntry->key = key;
	lruEntry->val = val;
	lruEntry->next = NULL;

	return lruEntry;
}

void movLRUCache(LRUEntry_t *prev) {
	// get the curr node
	LRUEntry_t *curr = prev->next;

	// check if the node already at tail
	if (curr == tailLRU) {
		return;
	}

	// unlink the current node
	prev->next = prev->next->next;

	// add current node at the tail
	tailLRU->next = curr;

	// update prev pointer of next node in LRU Hash Table
	getLRUHashTab(prev->next->key)->prev = prev;

	// update prev pointer of curr node in LRU Hash Table
	getLRUHashTab(curr->key)->prev = tailLRU;

	// set tail pointer to current node
	tailLRU = curr;
}

// get val with the key from LRU Cache
LRUEntry_t *getLRUCache(char *key) {
	// increase total_hits
	totalHits++;

	// check if the key exists
	LRUNode_t *lruNode = getLRUHashTab(key);

	// the key doesn't exist in LRU Cache
	if (lruNode == NULL) {
		return NULL;
	}

	// increase total_accesses;
	totalAccesses++;

	// get prev pointer and the value
	LRUEntry_t *prev = lruNode->prev;

	// move the node to the tail of the linked list
	movLRUCache(prev);

	return tailLRU;
}

// set lruEntry in LRU Cache
int setLRUCache(char *key, char *val) {
	// increase totalSetSuccess
	totalSetSuccess++;

	// check if the key exists in LRU Cache
	LRUEntry_t *curr = getLRUCache(key);

	// the key already exists in Cache
	if (curr != NULL) {
		// update the value with the key
		curr->val = val;
		return 0;
	}

	// if not reach maximum number of LRU Cache
	if (counterLRU < LRU_CACHE_SIZE) {
		// increase the counter
		counterLRU++;

		// create lruEntry
		LRUEntry_t *curr = createLRUEntry(key, val);

		// add curr node to the tail
		tailLRU->next = curr;

		// set the key and prev pointer into LRU Hash table
		setLRUHashTab(key, tailLRU);

		// move tail pointer to curr node
		tailLRU = curr;

		return 0;
	}

	// get the head of the LRU Cache
	LRUEntry_t *head = headLRU->next;

	// delete the key from LRU Hash table
	delLRUHashTab(head->key);

	// update the key and value
	head->key = key;
	head->val = val;

	// set key and prev pointer to LRU Hash Table
	setLRUHashTab(key, headLRU);

	// move curr node to the tail
	movLRUCache(headLRU);

	// increase totalEvictions
	totalEvictions++;

	return 0;
}

// delete the key in LRU Cache
int delLRUCache(char *key) {
	// check if the key exists
	LRUNode_t *lruNode = getLRUHashTab(key);

	// the key doesn't exist in LRU Cache
	if (lruNode == NULL) {
		return 1;
	}

	// get prev and curr pointer
	LRUEntry_t *prev = lruNode->prev;
	LRUEntry_t *curr = prev->next;

	// update
	if (curr == tailLRU) {
		// curr is the tail, update tail pointer
		tailLRU = prev;
	} else { // curr is not the tail, 
		// update prev pointer of next key in LRU Hash Table
		getLRUHashTab(curr->next->key)->prev = prev;
	}

	// delete curr key in LRU Hash table
	delLRUHashTab(key);

	// unlink the curr node
	prev->next = curr->next;

	// free memory of the curr node
	freemem((char *)curr, sizeof(LRUEntry_t));

	return 0;
}

/* ================= interfaces of KV Store ================= */

// set key and value pair into key-value store
int kv_set(char *key, char *val) {
	// set key and val pair into KV Hash table
	KVNode_t *kvNode = setKVHashTab(key, val);

	// get the allocated address of key and value
	char *keyAlloc = kvNode->key;
	char *valAlloc = kvNode->val;

	// set key and val into LRU Cache
	setLRUCache(keyAlloc, valAlloc);

	return 0;
}

// get value with the key from key-value store
char *kv_get(char *key) {
	// try get value with the key from LRU Cache
	LRUEntry_t *lruEntry = getLRUCache(key);

	// the key exists in LRU Cache, get the value
	if (lruEntry != NULL) {
		return lruEntry->val;
	}

	// try get the value directly from KV Hash Table
	KVNode_t *kvNode = getKVHashTab(key);

	// the key exists in KV Hash table, get it
	if (kvNode != NULL) {
		return kvNode->val;
	}

	return NULL;
}

int kv_delete(char *key) {
	// delete the key in LRU Cache
	delLRUCache(key);
	
	// delete the key in KV Hash Table
	int success = delKVHashTab(key);

	return success;
}

void kv_reset() {
	// reset LRU counter
	counterLRU = 0;
	totalHits = 0;            /* Total number of successful get requests */
	totalAccesses = 0;        /* Total number of get requests (including cache misses) */
	totalSetSuccess = 0;      /* Total number of successful set requests */
    // LRU_CACHE_SIZE         /* Total memory footprint of your key-value store */
    // counterLRU             /* Number of keys stored in the cache */
	totalEvictions = 0;       /* Number of items evicted */

	// clear LRU cache
	while (headLRU != NULL) {
		LRUEntry_t *currLRUEntry = headLRU;
		headLRU = headLRU->next;
		freemem((char *)currLRUEntry, sizeof(LRUEntry_t));
	}

	// reset headLRU and tail pointers
	headLRU = NULL;
	tailLRU = NULL;

	// initialize hash table
	int i;
	for (i = 0; i < HASH_TAB_SIZE; i++) {
		// clear kvNodes
		KVNode_t *kvHead = kvHashTab[i];
		while (kvHead != NULL) {
			KVNode_t *currKVNode = kvHead;
			kvHead = kvHead->next;

			// free memory of the key
			char *key = currKVNode->key;
			if (key != NULL) {
				int keyLen = strlen(key) + 1;
				freemem(key, keyLen);
			}

			// free memory of the value
			char *val = currKVNode->val;
			if (val != NULL) {
				xfree((void *)val);
			}

			// free memory of this kvNode
			freemem((char *)currKVNode, sizeof(KVNode_t));
		}

		// reset kv hash table head
		kvHashTab[i] = NULL;

		// clear lruNodes
		LRUNode_t *lruHead = lruHashTab[i];
		while (lruHead != NULL) {
			LRUNode_t *currLRUNode = lruHead;
			lruHead = lruHead->next;
			freemem((char *)currLRUNode, sizeof(LRUNode_t));
		}

		// reset lru hash table head
		lruHashTab[i] = NULL;
	}
}

int get_cache_info(char *kind) {
        // kind                - Value returned
        // "total_hits"        - Total number of successful get requests
        // "total_accesses"    - Total number of get requests (including cache misses, etc.)
        // "total_set_success" - Total number of successful set requests
        // "cache_size"        - Total memory footprint of your key-value store
        // "num_keys"          - Number of keys stored in the cache
        // "total_evictions"   - Number of items evicted

	if (strncmp(kind, "total_hits", 20) == 0) {

		// Total number of successful get requests
		return totalHits;

	} else if (strncmp(kind, "total_accesses", 20) == 0) {

		// Total number of get requests (including cache misses, etc.)
		return totalAccesses;

	} else if (strncmp(kind, "total_set_success", 20) == 0) {

		// Total number of successful set requests
		return totalSetSuccess;

	} else if (strncmp(kind, "cache_size", 20) == 0) {

		// Total memory footprint of your key-value store
		return LRU_CACHE_SIZE;

	} else if (strncmp(kind, "num_keys", 20) == 0) {

		// Number of keys stored in the cache
		return counterLRU;

	} else if (strncmp(kind, "total_evictions", 20) == 0) {

		// Number of items evicted
		return totalEvictions;

	}

	return -1;
}

char **most_popular_keys(int k) {
	char *temp = NULL;
	char **result = &temp;
	return result;
}
