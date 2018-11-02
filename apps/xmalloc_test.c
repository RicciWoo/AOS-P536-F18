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
		char *addr = (char *)xmalloc(size);
		if (addr == NULL) {
			printf("\nxmalloc failed, size: %d\n", size);
		} else {
			printf("%4d, ", size);
			if (i % 8 == 7) {
				printf("\n    ");
			}
		}

		// save the address
		*(bufptr + sizeof(char *) * i) = addr;
	}
	printf("\n");

	// show fragmentation information after allocation
	char *str = xheap_snapshot();
	printf("========= Fragmentation information after allocation =========\n");
	printf("%s\n", str);

	// randomly choose a buffer, then free it
	// show fragmentation information every 16 steps
	printf("========================= Random Free ========================\n");
	for (i = 0; i < ntest; i++) {
		// randomly choose one buffer
		int32 index = (int32)(rand() % ntest);
		char *ptr = *(bufptr + sizeof(char *) * index);
		if (ptr == NULL) {
			printf("    The buffer was not allocated!!!!\n");
			continue;
		}

		// free the selected buffer
		xfree((void *)ptr);
		printf("    free buffer at address: %d\n", *(bufptr + sizeof(char *) * index));
		*(bufptr + sizeof(char *) * index) = NULL;

		// show fragmentation information every 16 steps
		if (i % 16 == 15) {
			str = xheap_snapshot();
			printf("======== Fragmentation information after %d free steps ========\n", i);
			printf("%s\n", str);
		}
	}
}