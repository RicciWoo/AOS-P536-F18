#ifndef _KV_H_
#define _KV_H_


#define MAX_KEY_SIZE     64     /* maximum key size */
#define MAX_VAL_SIZE   1024     /* maximum value size */
#define HASH_TAB_SIZE   512     /* maximum number of keys */
#define LRU_CACHE_SIZE  128     /* maximum LRU cache size */

typedef struct KVNode {
    char *key;
    char *val;
    struct KVNode *next;
} KVNode_t;

typedef struct LRUEntry {
    char *key;
    char *val;
    struct LRUEntry *next;
} LRUEntry_t;

typedef struct LRUNode {
    char *key;
    LRUEntry_t *prev;
    struct LRUNode *next;
} LRUNode_t;

KVNode_t *kvHashTab[HASH_TAB_SIZE];

int counterLRU;
LRUEntry_t *headLRU, *tailLRU;
LRUNode_t *lruHashTab[HASH_TAB_SIZE];

// declaration of basic operations
int hashFunc(char *);         /* hash function */
char *kv_get(char *);         /* returns the value associated with the key. 
                                 If the key doesn't exist, returns NULL. */
int kv_set(char *, char *);   /* Returns false on error if unable to store. 
                                 Set operations either store a new key-value pair, or 
                                 overwrite the previous value associated with the key. */
int kv_delete(char *);        /* Removes key from the cache. 
                                 Returns false if key was not found, 
                                 and true for successful deletion. */
void kv_reset();              /* Deletes everything and resets everything. */
int kv_init();                /* Can be used for initializing your kv store. 
                                 You can pass arguments for selecting cache eviction, 
                                 buffer-pool configuration, etc. */
int get_cache_info(char *);   /* for getting information about the cache performance. 
                                 The function returns an output based on 
                                 the kind input string as follows: */
int totalHits;                /* Total number of successful get requests */
int totalAccesses;            /* Total number of get requests (including cache misses) */
int totalSetSuccess;          /* Total number of successful set requests */
    // LRU_CACHE_SIZE         /* Total memory footprint of your key-value store */
    // counterLRU             /* Number of keys stored in the cache */
int totalEvictions;           /* Number of items evicted */

char **most_popular_keys(int);/* Returns a sorted array of the k most popular keys. 
                                 Popularity depends on your cache eviction algorithm. */

void kv_test();        /* test function for key-value store */
void trace_small();
void trace_big();
void trace_long();
void trace_large();
void trace_xlarge();

#endif /* _KV_H_ */
