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
	char cArr[20] = "dummy return string";
	char *str = (char *)malloc(20);
	strncpy(str, cArr, size(cArr));
	return str;
}