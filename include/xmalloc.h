#ifndef _XMALLOC_H_
#define _XMALLOC_H_

#include <stdio.h>



// declaration of functions
void xmalloc_init();
void *xmalloc(size_t);
void xfree(void *);
char *xheap_snapshot();

#endif /* _XMALLOC_H_ */