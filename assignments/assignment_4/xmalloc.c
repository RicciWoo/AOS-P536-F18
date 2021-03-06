// copy of system/xmalloc.c
#include <xinu.h>
#include <xmalloc.h>

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
	bpid32	poolid;
	for (poolid = 0; poolid < poolnum; poolid++) {
		bufsize[poolid] = (minsize << poolid) - sizeof(int32);
		// set different buffer number for different size
		if (bufsize[poolid] <= 128) { // originally 128, changed for kv_test
			bufnumb[poolid] = 2048; // originally 64, changed for kv_test
		} else if (bufsize[poolid] <= 2048) {
			bufnumb[poolid] = 2048; // originally 32, changed for kv_test
		} else {
			bufnumb[poolid] = 2; // originally 16, changed for kv_test
		}
		// initialize allocated bytes and buffers
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
	}
}

void *xmalloc(int32 size) {
	if (size <= 0 || size > bufsize[nbpools - 1]) {
		return NULL;
	}

	// find the higer and closest buffer in size
	bpid32 poolid = findClosestIndex(size);
	if (poolid == (bpid32)SYSERR) {
		printf("findClosestIndex failed, size: %d\n", size);
		return NULL;
	}

	// allocate the buffer with the index
	struct bpentry *bpptr;
	bpptr = &buftab[poolid];
	if (bpptr->bpnext == NULL) {
		printf("no buffer with size %d is available!\n", bpptr->bpsize);
		return NULL;
	}
	char *bufptr = getbuf(poolid);
	if (bufptr == (char *)SYSERR) {
		printf("getbuf failed, poolid: %d\n", poolid);
		return NULL;
	}

	// save the allocated size at the end of the buffer
	char *addr = bufptr + bpptr->bpsize - sizeof(int32);
	*((int32 *)addr) = size;

	// update segmentation information
	allocBy[poolid] += size;
	allocBf[poolid]++;
	fragmBy[poolid] = bufsize[poolid] * allocBf[poolid] - allocBy[poolid];
	return (void *)bufptr;
}

void xfree(void *bufaddr) {
	if (bufaddr == NULL) {
		printf("Invalid address!\n");
		return;
	}

	// get the poolid of the pool
	char *addr = (char *)bufaddr - sizeof(bpid32);
	bpid32 poolid = *((bpid32 *)addr);
	if (poolid < 0 || poolid >= nbpools) {
		printf("Invalid poolid: %d\n", poolid);
		return;
	}

	// get the actual size that allocated before
	struct bpentry *bpptr = &buftab[poolid];
	addr += sizeof(bpid32) + bpptr->bpsize - sizeof(int32);
	int32 size = *((int32 *)addr);

	// give the buffer back to the pool
	syscall st = freebuf((char *)bufaddr);
	if (st == SYSERR) {
		printf("freebuf failed, address: %d", bufaddr);
		return;
	}

	// update segmentation information
	allocBy[poolid] -= size;
	allocBf[poolid]--;
	fragmBy[poolid] = bufsize[poolid] * allocBf[poolid] - allocBy[poolid];
}

char *xheap_snapshot() {
	// initialize fragmentation information string
	memset(fragStr, '\0', sizeof(fragStr));

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
	if (size <= 0 || size > bufsize[nbpools - 1]) {
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
