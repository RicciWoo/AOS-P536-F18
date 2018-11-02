#ifndef _XMALLOC_H_
#define _XMALLOC_H_

extern int32 bufsize[];
extern int32 bufnumb[];
extern int32 allocBy[];
extern int32 allocBf[];
extern int32 fragmBy[];
extern char  fragStr[];

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