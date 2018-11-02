#include <xinu.h>
#include <xmalloc.h>

void xmalloc_test() {
	// initialize segregated memory allocation
	xmalloc_init();
	
	// allocate ramdon size of buffers
	srand(97);
	int32 ntest = 64 + 32 + 16;
	// for holding all allocated addresses
	char **bufptr = (char **)getmem(ntest * sizeof(char *));
	int32 i;
	printf("========================= Allocation =========================\n    ");
	for (i = 0; i < ntest; i++) {
		int32 bpmaxb;
		if (i < 64) {
			bpmaxb = 128 - sizeof(int32);
		} else if (i < 96) {
			bpmaxb = 1024 - sizeof(int32);
		} else {
			bpmaxb = BP_MAXB - sizeof(int32);
		}
		int32 size = (int32)((rand() + BP_MINB) % bpmaxb);

		// allocate the buffer
		char *addr = NULL;
		if (size > 0) {
			addr = (char *)xmalloc(size);
		}
		printf("%3d=%4d, ", i, size);
		if (i % 8 == 7) {
			printf("\n    ");
		}

		// save the address
		printf("the allocated address: %d\n", addr);
		*(bufptr + sizeof(char *) * i) = addr;
	}
	printf("\n");

	for (i = 0; i < ntest; i++) {
		printf("the addresses in array: %d\n", *(bufptr + sizeof(char *) * i));
	}

	// show 
	bpid32 poolid;
	for (poolid = 0; poolid < nbpools; poolid++) {
		printf("poolid: %d, bufsize: %d, bufnumb: %d\n", poolid, bufsize[poolid], bufnumb[poolid]);
	}

	// for (poolid = 0; poolid < nbpools; poolid++) {
	// 	printf("poolid: %d, allocBy: %d, allocBf: %d, fragmBy: %d\n", poolid, allocBy[poolid], allocBf[poolid], fragmBy[poolid]);
	// }

	// for holding the fragmentation information
	char *str = (char *)getmem(sizeof(fragStr));

	// randomly choose a buffer, then free it
	// show fragmentation information every 16 steps
	printf("\n========================= Random Free ========================\n");
	for (i = 0; i < ntest; i++) {
		// show fragmentation information every 16 steps
		if (i % 16 == 0) {
			printf("\n");
			for (poolid = 0; poolid < nbpools; poolid++) {
				printf("poolid: %d, allocBy: %d, allocBf: %d, fragmBy: %d\n", poolid, allocBy[poolid], allocBf[poolid], fragmBy[poolid]);
			}
			char *retStr = xheap_snapshot();
			memcpy(str, retStr, sizeof(fragStr)); // copy for output
			printf("======== Fragmentation information after %d free steps ========\n", i);
			printf("%s\n", str);
		}

		// randomly choose one buffer
		int32 index = (int32)(rand() % ntest);
		char *ptr = *(bufptr + sizeof(char *) * index);
		if (ptr == NULL) {
			printf("    The buffer was not allocated, index: %d\n", index);
			continue;
		}

		// free the selected buffer
		xfree((void *)ptr);
		printf("    free buffer with index: %d\n", index);
		*(bufptr + sizeof(char *) * index) = NULL;
	}

	// show fragmentation information at the end
	printf("\n");
	for (poolid = 0; poolid < nbpools; poolid++) {
		printf("poolid: %d, allocBy: %d, allocBf: %d, fragmBy: %d\n", poolid, allocBy[poolid], allocBf[poolid], fragmBy[poolid]);
	}
	char *retStr = xheap_snapshot();
	memcpy(str, retStr, sizeof(fragStr)); // copy for output
	printf("\n============ Fragmentation information at the end ============\n");
	printf("%s\n", str);

	freemem((char *)bufptr, ntest * sizeof(char *));
	freemem((char *)str, sizeof(fragStr));
}