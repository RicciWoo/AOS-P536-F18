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

		hashCode = (hashCode * 31 + *key) % MAX_KEY_NUMB;
		key++;
	}

	return hashCode;
}

// kv store initialization
int kv_init() {
	// initialize segregated memory allocation
	xmalloc_init();

	// initialize the LRU counter
	countLRU = 0;

	// initialize head and tail pointer in LRU
	headLRU = (LRUNode_t *)xmalloc(sizeof(LRUNode_t));
	if (headLRU == NULL) {
		printf("error allocating memory for headLRU with size: %d\n", sizeof(LRUNode_t));
		return 0;
	}
	tailLRU = headLRU;

	// initialize hash table
	int i;
	for (i = 0; i < MAX_KEY_NUMB; i++) {
		// initialize the hash table of key-value pairs
		KVNode_t *kvNode = (KVNode_t *)xmalloc(sizeof(KVNode_t));
		if (kvNode == NULL) {
			printf("error allocating memory for head linked list in hash table with size: %d\n", 
				   sizeof(KVNode_t));
			return 0;
		}
		kvNode->key = NULL;
		kvNode->val = NULL;
		kvNode->next = NULL;
		hashTable[i] = kvNode;

		// initialize the hash table for LRU linked list
		LRUHash_t *lruNode = (LRUHash_t *)xmalloc(sizeof(LRUHash_t));
		if (lruNode == NULL) {
			printf("error allocating memory for head linked list in hash table with size: %d\n", 
				   sizeof(KVNode_t));
			return 0;
		}
		lruNode->key = NULL;
		lruNode->prev = NULL;
		lruNode->next = NULL;
		lruHash[i] = lruNode;
	}

	return 1;
}

// function for creating kvNode
KVNode_t *createKVNode(char *key, char *val) {
	// get length of key and value
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end
	int valLen = strlen(val) + 1; // +1 to hold '\0' at the end

	// allocation memory for key and value
	char *keyAlloc = (char *)xmalloc(keyLen);
	if (keyAlloc == NULL) {
		printf("error allocating memory for key with size: %d\n", keyLen);
		return NULL;
	}
	char *valAlloc = (char *)xmalloc(valLen);
	if (valAlloc == NULL) {
		printf("error allocating memory for value with size: %d\n", valLen);
		return NULL;
	}

	// initialize memrory for key and value
	memset((void *)keyAlloc, 0, keyLen);
	memset((void *)valAlloc, 0, valLen);

	// save key and value to the memory
	strncpy(keyAlloc, key, keyLen);
	strncpy(valAlloc, val, valLen);

	// allocate momory for kv node
	KVNode_t *kvNode = (KVNode_t *)xmalloc(sizeof(KVNode_t));
	if (kvNode == NULL) {
		printf("error allocating memory for kvNode with size: %d\n", 
			   sizeof(KVNode_t));
		return NULL;
	}

	// set allocated kv node
	kvNode->key = keyAlloc;
	kvNode->val = valAlloc;
	kvNode->next = NULL;

	return kvNode;
}

// insert kvNode into hash table
int insertHT(KVNode_t *kvNode) {
	// calculat hash code of the key
	char *key = kvNode->key;
	int hashCode = hashFunc(key);

	// get the head of the kv linked list
	KVNode_t *kvHead = hashTable[hashCode];

	// get length of key
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

	// traverse the linked list, check if the key not exist
	while (kvHead->next != NULL) {
		kvHead = kvHead->next;
		char *keyCurr = kvHead->key;
		if (strncmp(keyCurr, key, keyLen) == 0) {
			return 0;
		}
	}

	// insert the kvNode at the end
	kvHead->next = kvNode;

	return 1;
}

// function for creating lruNode
LRUNode_t *createLRUNode(char *key, char *val) {
	// allocate momory for kv node
	LRUNode_t *lruNode = (LRUNode_t *)xmalloc(sizeof(LRUNode_t));
	if (lruNode == NULL) {
		printf("error allocating memory for lruNode with size: %d\n", 
			   sizeof(LRUNode_t));
		return NULL;
	}

	// set allocated lru node
	lruNode->key = key;
	lruNode->val = val;
	lruNode->next = NULL;

	return lruNode;
}

// get previous LRUNode pointer from Hash table
LRUNode_t *getPrevHash(char *key) {
	// calculat hash code of the key
	int hashCode = hashFunc(key);

	// get the head of the lru linked list
	LRUHash_t *lruHead = lruHash[hashCode];

	// get length of key
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

	// check exist, then retrieve the prev pointer
	while (lruHead->next != NULL) {
		lruHead = lruHead->next;
		char *keyCurr = lruHead->key;
		if (strncmp(keyCurr, key, keyLen) == 0) {
			return lruHead->prev;
		}
	}

	return NULL;
}

LRUHash_t *getNodeHash(char *key) {
	// calculat hash code of the key
	int hashCode = hashFunc(key);

	// get the head of the LRU linked list
	LRUHash_t *lruHead = lruHash[hashCode];

	// get length of key
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

	// check not exist, then insert at the end
	while (lruHead->next != NULL) {
		lruHead = lruHead->next;
		char *keyCurr = lruHead->key;
		if (strncmp(keyCurr, key, keyLen) == 0) {
			return lruHead;
		}
	}

	return NULL;
}

// function for creating lruHash node
LRUHash_t *createLRUHash(char *key, LRUNode_t *prev) {
	// allocate momory for lruHash node
	LRUHash_t *lruHashNode = (LRUHash_t *)xmalloc(sizeof(LRUHash_t));
	if (lruHashNode == NULL) {
		printf("error allocating memory for lruHash with size: %d\n", 
			   sizeof(LRUHash_t));
		return NULL;
	}

	// set allocated lru node
	lruHashNode->key = key;
	lruHashNode->prev = prev;
	lruHashNode->next = NULL;

	return lruHashNode;
}

// insert lruHash node into LRU hash table
int insertLRUHash(LRUHash_t *lruHashNode) {
	// calculat hash code of the key
	char *key = lruHashNode->key;
	int hashCode = hashFunc(key);

	// get the head of the LRU linked list
	LRUHash_t *lruHead = lruHash[hashCode];

	// get length of key
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

	// traverse the linked list, check if the key not exist
	while (lruHead->next != NULL) {
		lruHead = lruHead->next;
		char *keyCurr = lruHead->key;
		if (strncmp(keyCurr, key, keyLen) == 0) {
			return 0;
		}
	}

	// insert the kvNode at the end
	lruHead->next = lruHashNode;

	return 1;
}

void moveToTail(LRUNode_t *prev) {
	// get the pointer to current node
	LRUNode_t *curr = prev->next;

	// is the tail, no need to move
	if (curr == tailLRU) {
		return;
	}

	// move the node to the tail
	prev->next = prev->next->next;
	tailLRU->next = curr;
	LRUHash_t *lruHashNode = getNodeHash(prev->next->key);
	lruHashNode->prev = prev;
	lruHashNode = getNodeHash(curr->key);
	lruHashNode->prev = tailLRU;
	tailLRU = curr;
}

// insert lruNode to LRU Cache
int insertLRU(KVNode_t *kvNode) {
	// get allocated key and value
	char *key = kvNode->key;
	char *val = kvNode->val;

	// check if the key exists in Cache
	LRUNode_t *prev = getPrevHash(key);

	// the key exist, return 0
	if (prev != NULL) {
		printf("error! the key already exists!\n");
		return 0;
	}

	// if cache size reach maximum, update the tail node
	if (countLRU >= MAX_LRU_SIZE) {
		// update the less recent used node
		LRUNode_t *lruHead = headLRU->next;
		lruHead->key = key;
		lruHead->val = val;

		// update LRU hash table
		LRUHash_t *lruHashNode = getNodeHash(key);
		lruHashNode->prev = headLRU;

		// move the new updated node to tail
		moveToTail(headLRU);
	}

	// create lruNode
	LRUNode_t *curr = createLRUNode(key, val);

	// set the curr node to the tail
	tailLRU->next = curr;

	// create LRU hash table entry for the new key
	LRUHash_t *lruHashNode = createLRUHash(key, tailLRU);

	// insert the entry to the hash table
	int success = insertLRUHash(lruHashNode);
	if (success == 0) {
		printf("error! the key already exists!\n");
		return 0;
	}

	// update the tail pointer
	tailLRU = curr;

	// increase counter
	countLRU++;
}

// set key-value pair
int kv_set(char *key, char *val) {
	//create kvNode
	KVNode_t *kvNode = createKVNode(key, val);

	// insert kvNode to hash table
	int success = insertHT(kvNode);
	if (success == 0) {
		printf("error! the key already exist!\n");
		return 0;
	}

	// insert lruNode to LRU Cache
	success = insertLRU(kvNode);
	if (success == 0) {
		printf("error! the key already exist!\n");
		return 0;
	}

	return 1;
}

// get value with the key from LRU
char *getValLRU(LRUNode_t *prev) {
	// move the node to the tail
	moveToTail(prev);

	return tailLRU->val;
}

// get value with the key from hash table
char *getValHT(char *key) {
	// calculat hash code of the key
	int hashCode = hashFunc(key);

	// get the head of the kv linked list
	KVNode_t *kvHead = hashTable[hashCode];

	// get length of key
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

	// check not exist, then insert at the end
	while (kvHead->next != NULL) {
		kvHead = kvHead->next;
		char *keyCurr = kvHead->key;
		if (strncmp(keyCurr, key, keyLen) == 0) {
			return kvHead->val;
		}
	}

	return NULL;
}

char *kv_get(char *key) {
	// check if the key exists in LRU
	LRUNode_t *prev = getPrevHash(key);

	// the key exists in LRU
	if (prev != NULL) {
		return getValLRU(prev);
	}

	// key doesn't exist in LRU, check if it in large Hash table
	return getValHT(key);
}

int kv_delete(char *key) {
	return 1;
}

void kv_reset() {
	// clear hash table
	for ()
}

int get_cache_info(char *kind) {
        // kind                - Value returned
        // "total_hits"        - Total number of successful get requests
        // "total_accesses"    - Total number of get requests (including cache misses, etc.)
        // "total_set_success" - Total number of successful set requests
        // "cache_size"        - Total memory footprint of your key-value store
        // "num_keys"          - Number of keys stored in the cache
        // "total_evictions"   - Number of items evicted

	return 1;
}

char **most_popular_keys(int k) {
	char *temp = NULL;
	return &temp;
}
