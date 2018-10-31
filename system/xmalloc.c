#include <xinu.h>
#include <xmalloc.h>

void xmalloc_init() {
	printf("start of void xmalloc_init()\n");
	int status = bufinit();
	if (status == SYSERR) {
		printf("bufinit failed\n");
		return;
	}
	int poolnum = 11;
	int minsize = 8;
	int bufsize[11];
	int i = 0;
	for (i = 0; i < poolnum; i++) {
		bufsize[i] = minsize << i;
		printf("bufsize #%d: %d/n", i, bufsize[i]);
	}
	bpid32 poolid = mkbufpool(32, 10);
	//restore(mask);
	printf("ID of pool that is created: %d\n", poolid);
	printf("end of void xmalloc_init()\n\n");
}

void *xmalloc(uint32 size) {
	printf("start of void *xmalloc(int)\n");
	return NULL;
	printf("end of void *xmalloc(int)\n\n");
}

void xfree(void *ptr) {
	printf("testing void xfree(void *)\n");
}

char *xheap_snapshot() {
	printf("testing char *xheap_snapshot()\n");
	char *str = (char *)getmem(20);
	strncpy(str, "dummy return string", 20);
	return str;
}