#include <xinu.h>
#include <xmalloc.h>

uint32 poolnum;
uint32 bufsize[NBPOOLS];
uint32 bufnumb[NBPOOLS];
uint32 allocBy[NBPOOLS];
uint32 allocBf[NBPOOLS];
char   compStr[NBPOOLS * 128];

void xmalloc_init() {
	// printf("start of void xmalloc_init()\n");

	status st = bufinit();
	if (st == SYSERR) {
		printf("bufinit failed!\n");
		return;
	}

	poolnum = 11;
	uint32 minsize = 8;
	uint32 maxnumb = 32;
	uint32 i = 0;
	for (i = 0; i < poolnum; i++) {
		bufsize[i] = minsize << i;
		//bufnumb[i] = maxnumb >> (i / 2);
		bufnumb[i] = maxnumb / 2;
		// printf("bufsize #%d: %d, ", i, bufsize[i]);
		// printf("bufnumb #%d: %d\n", i, bufnumb[i]);
		allocBy[i] = 0;
		allocBf[i] = 0;
	}

	memset(compStr, '\0', sizeof(compStr));

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
	// printf("start of void *xmalloc(int)\n");

	// find the higer and closest buffer in size
	bpid32 poolid = findClosestIndex(size);
	if (poolid == (bpid32)SYSERR) {
		printf("findClosestIndex failed, size: %d\n", size);
		return NULL;
	}
	// printf("the selected poolid for size %d is %d\n", size, poolid);

	// allocate the buffer with the index
	struct bpentry *bpptr;
	bpptr = &buftab[poolid];
	// printf("the buffer size of selected pool: %d\n", bpptr->bpsize);
	if (bpptr->bpnext == NULL) {
		printf("no buffer with size %d is available!\n", bpptr->bpsize);
		return NULL;
	}
	// printf("bpptr->bpnext before allocation: %d\n", bpptr->bpnext);
	char *bufptr = getbuf(poolid);
	if (bufptr == (char *)SYSERR) {
		printf("getbuf failed, poolid: %d\n", poolid);
		return NULL;
	}
	// printf("bpptr->bpnext after allocation: %d\n", bpptr->bpnext);

	// printf("end of void *xmalloc(int)\n\n");
	printf("allocted buffer with size: %d\n", bpptr->bpsize);
	allocBy[poolid] += size;
	allocBf[poolid]++;
	return (void *)bufptr;
}

void xfree(void *ptr) {
	// printf("start of void xfree(void *)\n");

	if (ptr == NULL) {
		printf("Invalid address!\n");
	}
	syscall st = freebuf((char *)ptr);
	if (st == SYSERR) {
		printf("freebuf failed, address: %d", ptr);
	}

	// printf("end of void xfree(void *)\n");
	printf("freed buffer at address: %d\n", ptr);
}

char *xheap_snapshot() {
	printf("start of char *xheap_snapshot()\n");

	bpid32 poolid = 0;
	char *strptr = &compStr[0];
	char temp[8];
	char *temptr = &temp[0];
	for (poolid = 0; poolid < nbpools; poolid++) {
		// pool_id=1, 
		strncat(strptr, "pool_id=", 8);
		sprintf(temptr, "%d", poolid);
		strncat(strptr, temptr, 8);
		strncat(strptr, ", ", 2);
		// buffer_size=32, 
		strncat(strptr, "buffer_size=", 14);
		sprintf(temptr, "%d", bufsize[poolid]);
		strncat(strptr, temptr, 8);
		strncat(strptr, ", ", 2);
		// total_buffers=20, 
		strncat(strptr, "total_buffers=");
		sprintf(temptr, "%d", bufnumb[poolid]);
		strncat(strptr, temptr, 8);
		strncat(strptr, ", ", 2);
		// allocated_bytes=243, 
		strncat(strptr, "allocated_bytes=");
		sprintf(temptr, "%d", allocBy[poolid]);
		strncat(strptr, temptr, 8);
		strncat(strptr, ", ", 2);
		// allocated_buffers=10, 
		strncat(strptr, "allocated_buffers=");
		sprintf(temptr, "%d", allocBf[poolid]);
		strncat(strptr, temptr, 8);
		strncat(strptr, ", ", 2);
		// fragmented_bytes=77 \n
		strncat(strptr, "fragmented_bytes=");
		uint32 fragbytes = bufsize[poolid] * allocBf[poolid] - allocBy[poolid];
		sprintf(temptr, "%d", fragbytes);
		strncat(strptr, temptr, 8);
		strncat(strptr, " \n", 2);
	}

	return &compStr[0];
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
