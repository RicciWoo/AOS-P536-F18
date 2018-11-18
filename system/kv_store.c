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

	// insert kvNode to LRU Cache

	return 1;
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
	

	char *val = getValHT(key);

	return val;
}

int kv_delete(char *key) {
	return 1;
}

void kv_reset() {

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
