#include <xinu.h>
#include <xmalloc.h>

void xmalloc_init() {
	printf("start of void xmalloc_init()\n");

	int status = bufinit();
	if (status == SYSERR) {
		printf("bufinit failed\n");
		return;
	}

	int poolnum = 10;
	int minsize = 8;
	int maxnumb = 32;
	int bufsize[10];
	int bufnumb[10];
	int i = 0;
	for (i = 0; i < poolnum; i++) {
		bufsize[i] = minsize << i;
		bufnumb[i] = maxnumb >> (i / 2);
		printf("bufsize #%d: %d, ", i, bufsize[i]);
		printf("bufnumb #%d: %d\n", i, bufnumb[i]);
	}

	bpid32 poolid = mkbufpool(32, 10);
	//restore(mask);
	printf("ID of pool that is created: %d\n", poolid);

	printf("end of void xmalloc_init()\n\n");
}

void *xmalloc(uint32 size) {
	printf("start of void *xmalloc(int)\n");

	printf("end of void *xmalloc(int)\n\n");
	return NULL;
}

void xfree(void *ptr) {
	printf("start of void xfree(void *)\n");

	printf("end of void xfree(void *)\n");
}

char *xheap_snapshot() {
	printf("start of char *xheap_snapshot()\n");
	char *str = (char *)getmem(20);
	strncpy(str, "dummy return string", 20);
	printf("end of char *xheap_snapshot()\n");
	return str;
}