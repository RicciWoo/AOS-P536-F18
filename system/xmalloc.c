#include <xinu.h>
#include <stdio.h>
#include <xmalloc.h>

void xmalloc_init() {
	printf("testing void xmalloc_init()\n");
}

void *xmalloc(int size) {
	printf("testing void *xmalloc(int)\n");
}

void xfree(void *ptr) {
	printf("testing void xfree(void *)\n");
}

char *xheap_snapshot() {
	printf("testing char *xheap_snapshot()\n");
	static char str[] = "dummy return string";
	return str;
}