#include <xinu.h>
#include <xmalloc.h>

int32 poolnum;
int32 bufsize[NBPOOLS];
int32 bufnumb[NBPOOLS];
int32 allocBy[NBPOOLS];
int32 allocBf[NBPOOLS];
int32 fragmBy[NBPOOLS];
char  fragStr[NBPOOLS * 128];

void xmalloc_init() {
	// initialize buffer pools
	status st = bufinit();
	if (st == SYSERR) {
		printf("bufinit failed!\n");
		return;
	}

	// setup buffer pools parameters
	poolnum = 10;
	int32 minsize = 16;
	int32 maxnumb = 32;
	bpid32	poolid;
	for (poolid = 0; poolid < poolnum; poolid++) {
		bufsize[poolid] = minsize << poolid;
		//bufnumb[poolid] = maxnumb >> (poolid / 2);
		bufnumb[poolid] = maxnumb / 2;
		// printf("bufsize #%d: %d, ", poolid, bufsize[poolid]);
		// printf("bufnumb #%d: %d\n", poolid, bufnumb[poolid]);
		allocBy[poolid] = 0;
		allocBf[poolid] = 0;
	}

	// make buffer pools
	for (poolid = 0; poolid < poolnum; poolid++) {
		// allocate additional space to store the actual size
		int32 bufsiz = bufsize[poolid] + sizeof(int32);
		bpid32 pid = mkbufpool(bufsiz, bufnumb[poolid]);
		if (pid == (bpid32)SYSERR) {
			printf("mkbufpool failed, bufsiz: %d, numbufs: %d\n", 
								bufsize[poolid], bufnumb[poolid]);
			return;
		}
		struct bpentry *bpptr;
		bpptr = &buftab[poolid];
		printf("poolid: %d, ", poolid);
		printf("bpptr->bpnext: %d, ", bpptr->bpnext);
		printf("bpptr->bpsize: %d\n", bpptr->bpsize);
	}

	// initialize fragmentation information string
	memset(fragStr, '\0', sizeof(fragStr));
}

void *xmalloc(int32 size) {
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

	printf("allocted buffer with size: %d\n", bpptr->bpsize);
	allocBy[poolid] += size;
	allocBf[poolid]++;
	fragmBy[poolid] = bufsize[poolid] * allocBf[poolid] - allocBy[poolid];
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
	// form the fragmentation information string
	bpid32 poolid = 0;
	char *strptr = &fragStr[0];
	char temp[8];
	char *temptr = &temp[0];
	for (poolid = 0; poolid < nbpools; poolid++) {
		// pool_id=1, 
		strncat(strptr, "pool_id=", 8);
		sprintf(temptr, "%d", poolid);
		strncat(strptr, temptr, 8);
		strncat(strptr, ", ", 2);
		// buffer_size=32, 
		strncat(strptr, "buffer_size=", 12);
		sprintf(temptr, "%d", bufsize[poolid]);
		strncat(strptr, temptr, 8);
		strncat(strptr, ", ", 2);
		// total_buffers=20, 
		strncat(strptr, "total_buffers=", 14);
		sprintf(temptr, "%d", bufnumb[poolid]);
		strncat(strptr, temptr, 8);
		strncat(strptr, ", ", 2);
		// allocated_bytes=243, 
		strncat(strptr, "allocated_bytes=", 16);
		sprintf(temptr, "%d", allocBy[poolid]);
		strncat(strptr, temptr, 8);
		strncat(strptr, ", ", 2);
		// allocated_buffers=10, 
		strncat(strptr, "allocated_buffers=", 18);
		sprintf(temptr, "%d", allocBf[poolid]);
		strncat(strptr, temptr, 8);
		strncat(strptr, ", ", 2);
		// fragmented_bytes=77 \n
		strncat(strptr, "fragmented_bytes=", 17);
		sprintf(temptr, "%d", fragmBy[poolid]);
		strncat(strptr, temptr, 8);
		strncat(strptr, " \n", 2);
	}

	return &fragStr[0];
}

// find first index of size greater or equals to size
bpid32 findClosestIndex(int32 size) {
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
