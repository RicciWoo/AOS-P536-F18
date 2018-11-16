#include <xinu.h>
#include <kv.h>
#include <xmalloc.h>

int hashFunc(char *key) {
	if (key == NULL || *key == '\0') {
		return -1;
	}

	int hashCode = 0;
	for (int i = 0; i < MAX_KEY_SIZE; i++) {
		if (*key == '\0') {
			break;
		}

		hoshCode = (hashCode * 31 + *key) % MAX_KEY_NUMB;
		key++;
	}

	return hashCode;
}

int kv_init() {
	// initialize the hash map of key-value pairs
	KVNode_t *hashTablePtr = &hashTable[0];
	memset((void *)hashTablePtr, 0, MAX_KEY_NUMB);

	// initialize the hash map for LRU linked list
	LRUNode_t *lruCachePtr = &lruCache[0];
	memset((void *)lruCachePtr, 0, MAX_KEY_NUMB);

	return 1;
}

int kv_set(char *key, char *val) {
	// get length of key and value
	int keyLen = strlen(key);
	int valLen = strlen(val);

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











