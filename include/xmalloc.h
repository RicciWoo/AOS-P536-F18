#ifndef _XMALLOC_H_
#define _XMALLOC_H_




// declaration of functions
void xmalloc_init();
void *xmalloc(int);
void xfree(void *);
char *xheap_snapshot();

void xmalloc_test();

#endif /* _XMALLOC_H_ */