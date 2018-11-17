#include <xinu.h>
#include <kv.h>
#include <xmalloc.h>


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

int kv_init() {
	// initialize segregated memory allocation
	xmalloc_init();

	// initialize hash table
	int i;
	for (i = 0; i < MAX_KEY_NUMB; i++) {
		// initialize the hash table of key-value pairs
		KVNode_t * kvNode = xmalloc(sizeof(KVNode_t));
		kvNode->keyPtr = NULL;
		kvNode->valPtr = NULL;
		kvNode->next = NULL;
		hashTable[i] = kvNode;

		// initialize the hash table for LRU linked list
		LRUNode_t * lruNode = xmalloc(sizeof(LRUNode_t));
		lruNode->keyPtr = NULL;
		lruNode->valPtr = NULL;
		lruNode->prev = NULL;
		lruCache[i] = NULL;
	}

	return 1;
}

int insertHT(KVNode_t *kvNode) {
	// calculat hash code of the key
	char *keyPtr = kvNode->keyPtr;
	int hashCode = hashFunc(keyPtr);

	// get the head of the kv linked list
	KVNode_t *kvHead = hashTable[hashCode];

	// get length of key
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end

	// check not exist, then insert at the end
	while (kvHead->next != NULL) {
		char *keyNode = kvHead->next->keyPtr;
		if (strncmp(keyNode, keyPtr, keyLen) == 0) {
			return 0;
		}
		kvHead = kvHead->next;
	}
	kvHead->next = kvNode;

	return 1;
}

int kv_set(char *key, char *val) {
	// get length of key and value
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end
	int valLen = strlen(val) + 1; // +1 to hold '\0' at the end

	// allocation memory for key and value
	char *keyPtr = xmalloc(keyLen);
	char *valPtr = xmalloc(valLen);

	// initialize memrory for key and value
	memset((void *)keyPtr, 0, keyLen);
	memset((void *)valPtr, 0, valLen);

	// save key and value to the memory
	strncpy(keyPtr, key, keyLen);
	strncpy(valPtr, val, valLen);

	// save addresses to the kv node
	KVNode_t *kvNode = (KVNode_t *)xmalloc(sizeof(KVNode_t));
	kvNode->keyPtr = keyPtr;
	kvNode->valPtr = valPtr;
	kvNode->next = NULL;

	// insert kv node to hash table linked list
	int status = insertHT(kvNode);
	if (status == 0) {
		printf("error inserting node to hash table!\n");
	}

	int hashCode = hashFunc(keyPtr);
	KVNode_t *kvHead = hashTable[hashCode];
	printf("check hash table: \n");
	while (kvHead->next != NULL) {
		printf("key: %s, val: %s\n", kvHead->next->keyPtr, kvHead->next->valPtr);
		kvHead = kvHead->next;
	}

	return 1;
}

char *kv_get(char *key) {
	return NULL;
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
