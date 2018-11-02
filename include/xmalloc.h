#ifndef _XMALLOC_H_
#define _XMALLOC_H_

extern uint32 bufsize[];
extern uint32 bufnumb[];
extern uint32 allocBy[];
extern uint32 allocBf[];

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