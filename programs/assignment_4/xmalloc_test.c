// copy of apps/xmalloc_test.c
#include <xinu.h>
#include <xmalloc.h>

void xmalloc_test() {
	// initialize segregated memory allocation
	xmalloc_init();
	
	// allocate ramdon size of buffers
	srand(97);
	// int32 ntest = 64 + 32 + 16; // original
	int32 ntest = 2048 + 1024 + 2; // changed for kv_test
	// for holding all allocated addresses
	addr_t bufptr[ntest];
	int32 i;
	printf("========================= Allocation =========================\n    ");
	for (i = 0; i < ntest; i++) {
		int32 bpmaxb;
		if (i < 2048) { // initially 64, changed for kv_test
			bpmaxb = 32 - sizeof(int32); // initially 128, changed for kv_test
		} else if (i < 3072) { // initially 96, changed for kv_test
			bpmaxb = 2048 - sizeof(int32); // initially 1024, changed for kv_test
		} else {
			bpmaxb = BP_MAXB - sizeof(int32);
		}
		int32 size = (int32)((rand() + BP_MINB) % bpmaxb);

		// allocate the buffer
		addr_t addr = NULL;
		if (size > 0) {
			addr = (addr_t)xmalloc(size);
		}
		printf("%4d, ", size);
		if (i % 8 == 7) {
			printf("\n    ");
		}

		// save the address
		bufptr[i] = addr;
	}
	printf("\n");

	// for holding the fragmentation information
	char *str = (char *)getmem(sizeof(fragStr));

	// randomly choose a buffer, then free it
	// show fragmentation information every 16 steps
	printf("\n========================= Random Free ========================\n");
	for (i = 0; i < ntest; i++) {
		// show fragmentation information every 16 steps
		if (i % 16 == 0) {
			char *retStr = xheap_snapshot();
			memcpy(str, retStr, sizeof(fragStr)); // copy for output
			printf("\n======== Fragmentation information after %d free steps ========\n", i);
			printf("%s\n", str);
		}

		// randomly choose one buffer
		int32 index = (int32)(rand() % ntest);
		addr_t ptr = bufptr[index];

		if (ptr == NULL) {
			printf("    The buffer was not allocated, index: %d\n", index);
			continue;
		}

		// free the selected buffer
		xfree((void *)ptr);
		printf("    free buffer with index: %d\n", index);
		bufptr[index] = NULL;
	}

	// show fragmentation information at the end
	char *retStr = xheap_snapshot();
	memcpy(str, retStr, sizeof(fragStr)); // copy for output
	printf("\n============ Fragmentation information at the end ============\n");
	printf("%s\n", str);

	freemem((char *)str, sizeof(fragStr));
}
