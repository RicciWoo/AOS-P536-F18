## Assignment 4: Memory allocation

#### Table of Contents
1. Segregated Memory Allocation
	1. Additional requirements for 536/599 students
	2. References
	3. Hints
	4. Deliverables
	5. Grading

#### Segregated Memory Allocation
In this assignment, you will be building a general purpose memory allocator for XINU.

Your allocator should use the segregated memory allocation principles discussed in class. That is, you should allocate memory of different sizes from different memory areas. To do so, you can use XINU's buffer pools for the lower-level memory allocation.

Your task is to implement an allocator similar to malloc. You will need to implement these calls:

    void xmalloc_init() ;

    void* ptr = xmalloc(size_t) ;

    void xfree(void* ptr) ;

The xmalloc_init routine can be used to initialize your allocator and set up the necessary data structures.

#### Additional requirements for 536/599 students

In addition to the memory allocation routines mentioned above, you will also need to implement heap instrumentation routines for getting information about the fragmentation. In particular, you will need to implement:

    char* xheap_snapshot()

This call should return a text string which contains information about the occupancy of each buffer pool, at that point in time.  
For instance, an example output can be:

    pool_id=1, buffer_size=32, total_buffers=20, allocated_bytes=243, allocated_buffers=10, fragmented_bytes=77 \n
    pool_id=2, buffer_size=64, total_buffers=10, allocated_bytes=100, allocated_buffers=2, fragmented_bytes=28 \n
    ...

Naturally, the heap's composition will change over time due to allocations and deallocations, and the output of the heap snapshot will change.

#### References

1. The Memory Fragmentation Problem: Solved? Mark S. Johnstone Paul R. Wilson
https://www.cs.tufts.edu/~nr/cs257/archive/paul-wilson/fragmentation.pdf
2. Doug Lea Malloc http://g.oswego.edu/dl/html/malloc.html

#### Hints

1. Think carefully of how to keep track of allocated objects, since you will need to free them eventually.
2. It might be useful to tune the size of buffers and number of buffers. Powers of 2 might work best, or not.

#### Deliverables

1. Report explaining design
2. You must provide a sample test-case file, named xmalloc_test.c, that makes multiple xmalloc/xfree calls.

#### Grading

1. Report: 20%
2. Allocator implementation: 80%
	1. Low fragmentation
	2. Objects actually freed
3. xheap_snapshot : 20%

