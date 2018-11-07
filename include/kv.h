#ifndef _KV_H_
#define _KV_H_



// declaration of basic operations
char *kv_get(char *);         /* returns the value associated with the key. 
                                 If the key doesn't exist, returns NULL. */
bool kv_set(char *, char *);  /* Returns false on error if unable to store. 
                                 Set operations either store a new key-value pair, or 
                                 overwrite the previous value associated with the key. */
bool kv_delete(char *);       /* Removes key from the cache. 
                                 Returns false if key was not found, 
                                 and true for successful deletion. */
void kv_reset();              /* Deletes everything and resets everything. */
int kv_init();                /* Can be used for initializing your kv store. 
                                 You can pass arguments for selecting cache eviction, 
                                 buffer-pool configuration, etc. */
int get_cache_info(char *);   /* for getting information about the cache performance. 
                                 The function returns an output based on 
                                 the kind input string as follows: */
        // kind                - Value returned
        // "total_hits"        - Total number of successful get requests
        // "total_accesses"    - Total number of get requests (including cache misses, etc.)
        // "total_set_success" - Total number of successful set requests
        // "cache_size"        - Total memory footprint of your key-value store
        // "num_keys"          - Number of keys stored in the cache
        // "total_evictions"   - Number of items evicted

char **most_popular_keys(int);/* Returns a sorted array of the k most popular keys. 
                                 Popularity depends on your cache eviction algorithm. */

#endif /* _KV_H_ */