#include <xinu.h>
#include <xmalloc.h>

int32 bufsize[NBPOOLS];
int32 bufnumb[NBPOOLS];

void xmalloc_init() {
	// printf("start of void xmalloc_init()\n");

	status st = bufinit();
	if (st == SYSERR) {
		printf("bufinit failed!\n");
		return;
	}

	int32 poolnum = 11;
	int32 minsize = 8;
	int32 maxnumb = 32;
	int32 i = 0;
	for (i = 0; i < poolnum; i++) {
		bufsize[i] = minsize << i;
		bufnumb[i] = maxnumb >> (i / 2);
		// printf("bufsize #%d: %d, ", i, bufsize[i]);
		// printf("bufnumb #%d: %d\n", i, bufnumb[i]);
	}

	for (i = 0; i < poolnum; i++) {
		bpid32 poolid = mkbufpool(bufsize[i], bufnumb[i]);
		if (poolid == (bpid32)SYSERR) {
			printf("mkbufpool failed, bufsiz: %d, numbufs: %d\n", bufsize[i], bufnumb[i]);
			return;
		}
		// struct bpentry *bpptr;
		// bpptr = &buftab[poolid];
		// printf("poolid: %d, ", poolid);
		// printf("bpptr->bpnext: %d, ", bpptr->bpnext);
		// printf("bpptr->bpsize: %d\n", bpptr->bpsize);
	}

	// printf("end of void xmalloc_init()\n\n");
}

void *xmalloc(uint32 size) {
	printf("start of void *xmalloc(int)\n");

	// find the higer and closest buffer in size
	bpid32 poolid = findClosestIndex(size);
	if (poolid == (bpid32)SYSERR) {
		printf("findClosestIndex failed, size: %d\n", size);
		return NULL;
	}
	printf("the selected poolid for size %d is %d\n", size, poolid);

	// allocate the buffer with the index
	struct bpentry *bpptr;
	bpptr = &buftab[poolid];
	printf("the buffer size of selected pool: %d\n", bpptr->bpsize);
	printf("bpptr->bpnext before allocation: %d\n", bpptr->bpnext);
	char *bufptr = getbuf(poolid);
	if (bufptr == (char *)SYSERR) {
		printf("getbuf failed, poolid: %d\n", poolid);
		return NULL;
	}
	if (bufptr == NULL) {
		printf("no buffer in the pool is available!\n");
	} else {
		printf("bpptr->bpnext after allocation: %d\n", bpptr->bpnext);
	}
	

	printf("end of void *xmalloc(int)\n\n");
	return (void *)bufptr;
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

// find first index of size greater or equals to size
bpid32 findClosestIndex(uint32 size) {
	if (size > bufsize[nbpools - 1]) {
		return (bpid32)SYSERR;
	}
	if (size <= bufsize[0]) {
		return 0;
	}

	bpid32 start = 0, end = nbpools - 1;
	while (start + 1 < end) {
		bpid32 mid = start + (end - start) / 2;
		if (bufsize[mid] < size) {
			start = mid;
		} else {
			end = mid;
		}
	}

	if (bufsize[start] >= size) {
		return start;
	}
	if (bufsize[end] >= size) {
		return end;
	}
	return (bpid32)SYSERR;
}
