#include <xinu.h>
#include <stdio.h>
#include <xmalloc.h>

void xmalloc_test() {
	printf("start testing the xmalloc...\n");
	xmalloc_init();
	int *ptr = (int *)xmalloc(28);
	xfree((void *)ptr);
	char *str = xheap_snapshot();
	printf("the return string: %s\n", str);
	printf("this is the end of testing.\n");
}