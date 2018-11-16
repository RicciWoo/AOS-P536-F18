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

	// initialize the hash map of key-value pairs
	KVNode_t *hashTablePtr = &hashTable[0];
	memset((void *)hashTablePtr, 0, MAX_KEY_NUMB);

	// initialize the hash map for LRU linked list
	LRUNode_t *lruCachePtr = &lruCache[0];
	memset((void *)lruCachePtr, 0, MAX_KEY_NUMB);

	return 1;
}

int kv_set(char *key, char *val) {
	printf("the key is: %s\n", key);
	printf("the val is: %s\n", val);

	// get length of key and value
	int keyLen = strlen(key) + 1; // +1 to hold '\0' at the end
	int valLen = strlen(val) + 1; // +1 to hold '\0' at the end
	printf("length of key: %d\n", keyLen);
	printf("length of val: %d\n", valLen);

	// allocation memory for key and value
	char *keyPtr = xmalloc(keyLen);
	char *valPtr = xmalloc(valLen);
	printf("allocated key address: %d\n", keyPtr);
	printf("allocated val address: %d\n", valPtr);

	// initialize memrory for key and value
	memset((void *)keyPtr, 0, keyLen);
	memset((void *)valPtr, 0, valLen);
	printf("the initial key is: %s\n", keyPtr);
	printf("the initial val is: %s\n", valPtr);

	// save key and value to the memory
	strncpy(keyPtr, key, keyLen);
	strncpy(valPtr, val, valLen);
	printf("the saved key is: %s\n", keyPtr);
	printf("the saved val is: %s\n", valPtr);

	// calculat hash code of the key
	int hashCode = hashFunc(keyPtr);
	printf("hash code of the key: %d\n", hashCode);



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
