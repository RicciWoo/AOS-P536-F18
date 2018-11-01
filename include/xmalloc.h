#ifndef _XMALLOC_H_
#define _XMALLOC_H_

extern int32 bufsize[];
extern int32 bufnumb[];

extern void srand(unsigned long);
extern unsigned long rand(void);

// declaration of functions
void xmalloc_init();
void *xmalloc(uint32);
void xfree(void *);
char *xheap_snapshot();
bpid32 findClosestIndex(uint32);

void xmalloc_test();

#endif /* _XMALLOC_H_ */