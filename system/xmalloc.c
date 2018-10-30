#include <xinu.h>
#include <xmalloc.h>

void xmalloc_init() {
	printf("start of void xmalloc_init()\n");
	//intmask mask = disable();
	bpid32 poolid = mkbufpool(32, 10);
	//restore(mask);
	printf("ID of pool that is created: %d\n", poolid);
	printf("end of void xmalloc_init()\n");
}

void *xmalloc(uint32 size) {
	printf("start of void *xmalloc(int)\n");

	printf("end of void *xmalloc(int)\n");
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