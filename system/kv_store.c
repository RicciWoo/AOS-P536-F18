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
int setKVHashTab(char *key, char *val) {
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

		return 0;
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

	return 0;
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

// // function for creating lruNode
// LRUNode_t *createLRUNode(char *key, char *val) {
// 	// allocate momory for kv node
// 	LRUNode_t *lruNode = (LRUNode_t *)xmalloc(sizeof(LRUNode_t));
// 	if (lruNode == NULL) {
// 		printf("error allocating memory for lruNode with size: %d\n", 
// 			   sizeof(LRUNode_t));
// 		return NULL;
// 	}

// 	// set allocated lru node
// 	lruNode->key = key;
// 	lruNode->val = val;
// 	lruNode->next = NULL;

// 	return lruNode;
// }

// // get previous LRUNode pointer from Hash table
// LRUNode_t *getPrevHash(char *key) {
// 	// calculat hash code of the key
// 	int hashCode = hashFunc(key);

// 	// get the head of the lru linked list
// 	LRUHash_t *lruHead = lruHash[hashCode];

// 	// get length of key
// 	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

// 	// check exist, then retrieve the prev pointer
// 	while (lruHead->next != NULL) {
// 		lruHead = lruHead->next;
// 		char *keyCurr = lruHead->key;
// 		if (strncmp(keyCurr, key, keyLen) == 0) {
// 			return lruHead->prev;
// 		}
// 	}

// 	return NULL;
// }

// LRUHash_t *getNodeHash(char *key) {
// 	// calculat hash code of the key
// 	int hashCode = hashFunc(key);

// 	// get the head of the LRU linked list
// 	LRUHash_t *lruHead = lruHash[hashCode];

// 	// get length of key
// 	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

// 	// check not exist, then insert at the end
// 	while (lruHead->next != NULL) {
// 		lruHead = lruHead->next;
// 		char *keyCurr = lruHead->key;
// 		if (strncmp(keyCurr, key, keyLen) == 0) {
// 			return lruHead;
// 		}
// 	}

// 	return NULL;
// }

// // function for creating lruHash node
// LRUHash_t *createLRUHash(char *key, LRUNode_t *prev) {
// 	// allocate momory for lruHash node
// 	LRUHash_t *lruHashNode = (LRUHash_t *)xmalloc(sizeof(LRUHash_t));
// 	if (lruHashNode == NULL) {
// 		printf("error allocating memory for lruHash with size: %d\n", 
// 			   sizeof(LRUHash_t));
// 		return NULL;
// 	}

// 	// set allocated lru node
// 	lruHashNode->key = key;
// 	lruHashNode->prev = prev;
// 	lruHashNode->next = NULL;

// 	return lruHashNode;
// }

// // insert lruHash node into LRU hash table
// int insertLRUHash(LRUHash_t *lruHashNode) {
// 	// calculat hash code of the key
// 	char *key = lruHashNode->key;
// 	int hashCode = hashFunc(key);

// 	// get the head of the LRU linked list
// 	LRUHash_t *lruHead = lruHash[hashCode];

// 	// get length of key
// 	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

// 	// traverse the linked list, check if the key not exist
// 	while (lruHead->next != NULL) {
// 		lruHead = lruHead->next;
// 		char *keyCurr = lruHead->key;
// 		if (strncmp(keyCurr, key, keyLen) == 0) {
// 			return 0;
// 		}
// 	}

// 	// insert the kvNode at the end
// 	lruHead->next = lruHashNode;

// 	return 1;
// }

// void moveToTail(LRUNode_t *prev) {
// 	// get the pointer to current node
// 	LRUNode_t *curr = prev->next;

// 	// is the tail, no need to move
// 	if (curr == tailLRU) {
// 		return;
// 	}

// 	// move the node to the tail
// 	prev->next = prev->next->next;
// 	tailLRU->next = curr;
// 	LRUHash_t *lruHashNode = getNodeHash(prev->next->key);
// 	lruHashNode->prev = prev;
// 	lruHashNode = getNodeHash(curr->key);
// 	lruHashNode->prev = tailLRU;
// 	tailLRU = curr;
// }

// // insert lruNode to LRU Cache
// int insertLRU(KVNode_t *kvNode) {
// 	// get allocated key and value
// 	char *key = kvNode->key;
// 	char *val = kvNode->val;

// 	// check if the key exists in Cache
// 	LRUNode_t *prev = getPrevHash(key);

// 	// the key exist, return 0
// 	if (prev != NULL) {
// 		printf("error! the key already exists!\n");
// 		return 0;
// 	}

// 	// if cache size reach maximum, update the tail node
// 	if (countLRU >= MAX_LRU_SIZE) {
// 		// update the less recent used node
// 		LRUNode_t *lruHead = headLRU->next;
// 		lruHead->key = key;
// 		lruHead->val = val;

// 		// update LRU hash table
// 		LRUHash_t *lruHashNode = getNodeHash(key);
// 		lruHashNode->prev = headLRU;

// 		// move the new updated node to tail
// 		moveToTail(headLRU);
// 	}

// 	// create lruNode
// 	LRUNode_t *curr = createLRUNode(key, val);

// 	// set the curr node to the tail
// 	tailLRU->next = curr;

// 	// create LRU hash table entry for the new key
// 	LRUHash_t *lruHashNode = createLRUHash(key, tailLRU);

// 	// insert the entry to the hash table
// 	int success = insertLRUHash(lruHashNode);
// 	if (success == 0) {
// 		printf("error! the key already exists!\n");
// 		return 0;
// 	}

// 	// update the tail pointer
// 	tailLRU = curr;

// 	// increase counter
// 	countLRU++;
// }

// set key and value pair into key-value store
int kv_set(char *key, char *val) {
// 	//create kvNode
// 	KVNode_t *kvNode = createKVNode(key, val);

// 	// insert kvNode to hash table
// 	int success = insertHT(kvNode);
// 	if (success == 0) {
// 		printf("error! the key already exist!\n");
// 		return 0;
// 	}

// 	// insert lruNode to LRU Cache
// 	success = insertLRU(kvNode);
// 	if (success == 0) {
// 		printf("error! the key already exist!\n");
// 		return 0;
// 	}

	return 1;
}

// // get value with the key from LRU
// char *getValLRU(LRUNode_t *prev) {
// 	// move the node to the tail
// 	moveToTail(prev);

// 	return tailLRU->val;
// }

// // get value with the key from hash table
// char *getValHT(char *key) {
// 	// calculat hash code of the key
// 	int hashCode = hashFunc(key);

// 	// get the head of the kv linked list
// 	KVNode_t *kvHead = hashTable[hashCode];

// 	// get length of key
// 	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

// 	// check not exist, then insert at the end
// 	while (kvHead->next != NULL) {
// 		kvHead = kvHead->next;
// 		char *keyCurr = kvHead->key;
// 		if (strncmp(keyCurr, key, keyLen) == 0) {
// 			return kvHead->val;
// 		}
// 	}

// 	return NULL;
// }

// get value with the key from key-value store
char *kv_get(char *key) {
// 	// check if the key exists in LRU
// 	LRUNode_t *prev = getPrevHash(key);

// 	// the key exists in LRU
// 	if (prev != NULL) {
// 		return getValLRU(prev);
// 	}

// 	// key doesn't exist in LRU, check if it in large Hash table
// 	return getValHT(key);
	return NULL;
}

// int kv_delete(char *key) {
// 	return 1;
// }

void kv_reset() {
	// reset LRU counter
	counterLRU = 0;

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

	return 1;
}

char **most_popular_keys(int k) {
	char *temp = NULL;
	return &temp;
}
