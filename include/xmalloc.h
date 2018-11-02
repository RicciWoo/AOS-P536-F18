#ifndef _XMALLOC_H_
#define _XMALLOC_H_

typedef	char *addr_t;

int32 poolnum;                // total number of buffer pools
int32 bufsize[NBPOOLS];       // buffer size in each pool
int32 bufnumb[NBPOOLS];       // number of buffers in each pool
int32 allocBy[NBPOOLS];       // allocated bytes for each pool
int32 allocBf[NBPOOLS];       // allocated buffers for each pool
int32 fragmBy[NBPOOLS];       // fragmented bytes in each pool
char  fragStr[NBPOOLS * 128]; // the fragmentation information string

extern void srand(unsigned long);
extern unsigned long rand(void);

// declaration of functions
void xmalloc_init();
void *xmalloc(int32);
void xfree(void *);
char *xheap_snapshot();
bpid32 findClosestIndex(int32);

void xmalloc_test();

#endif /* _XMALLOC_H_ */