## Assignment 5: Key value store

#### Table of Contents
1. In-memory Key-value Store
	1. Basic Operations
	2. Cache Monitoring Operations
	3. Cache Eviction
	4. Design Considerations and Hints
	5. Grading

#### In-memory Key-value Store

Key-value stores are an essential building block of modern systems. They serve both as data-storage and caching layers. Memcached, Redis, and Cassandra are a few popular examples.

In this assignment, you will be creating your own in-memory key-value store in Xinu. As the name suggests, in-memory ke-value stores cache commonly accessed keys and their associated values in memory. Applications can store key-value pairs by calling set(key, value), and retrieve them by calling get(key). Both keys and values are standard NULL terminated strings.

Thus you will basically be implementing a large in-memory cache. New items are added using set operations, and items are retrieved using get. Clearly, keeping track of the items is a crucial task, for which you can use a large hash-table that indexes the keys.

#### Basic Operations

An in-memory key-value store has these main operations which you will be required to implement.

1. char* value = kv_get(key) returns the value associated with the key. If the key doesn't exist, returns NULL.

2. bool kv_set(key, value) . Returns false on error if unable to store. Set operations either store a new key-value pair, or overwrite the previous value associated with the key.

3. bool kv_delete(key) . Removes key from the cache. Returns false if key was not found, and true for successful deletion.

4. void kv_reset() Deletes everything and resets everything.

5. int kv_init(...) Can be used for initializing your kv store. You can pass arguments for selecting cache eviction, buffer-pool configuration, etc.

As you probably can already see, careful memory allocation and management is the core of this assignment. Therefore, you can (and should) use the memory allocator built in the previous assignment to allocate memory for all the keys and values and other indices.

These functions should be declared in include/kv.h .

#### Cache Monitoring Operations

Furthermore, you will be required to implement some cache monitoring operations:

1. int get_cache_info(char* kind) should be the top-level function for getting information about the cache performance. The function returns an output based on the kind input string as follows:

| kind                | Value returned                         |
|:--------------------|:---------------------------------------|
| "total_hits"        | Total number of successful get requests|
| "total_accesses"    | Total number of get requests (including cache misses, etc.)|
| "total_set_success" | Total number of successful set requests|
| "cache_size"        | Total memory footprint of your key-value store|
| "num_keys"          | Number of keys stored in the cache     |
| "total_evictions"   | Number of items evicted                |

2. char** most_popular_keys(int k) Returns a sorted array of the k most popular keys. Popularity depends on your cache eviction algorithm. For example, if you are using LRU, then return the k most recently read keys. Similarly, if you are using LFU, then return the k most frequently read keys.

#### Cache Eviction

Eventually your in-memory store will run out of memory, and you will have to evict some items from the cache. You will therefore need to implement some cache eviction policy. You can implement some variant of LRU or LFU, or fancier if you prefer. As a bonus, you can also implement ARC cache replacement, which uses four lists to implement an LRU-LFU hybrid.

#### Design Considerations and Hints

1. You should limit the maximum key and value sizes. Max key size will be 64bytes. Max value sizes will be 1KB.

2. Think about how to treat large keys and values. Truncation? Ignore? Throw error?

3. The memory for the values needs to come from your memory allocator. Keys are generally smaller in size so you could technically use them directly as an index into the hash-table.

#### Grading

1. Basic set and get: 30%
2. Cache monitoring: 20%
3. Cache eviction: 30%
4. Report: 20%
5. Bonus: ARC cache replacement: 20%
