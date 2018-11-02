#ifndef _XMALLOC_H_
#define _XMALLOC_H_

int32 poolnum;
int32 bufsize[NBPOOLS];
int32 bufnumb[NBPOOLS];
int32 allocBy[NBPOOLS];
int32 allocBf[NBPOOLS];
int32 fragmBy[NBPOOLS];
char  fragStr[NBPOOLS * 128];

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