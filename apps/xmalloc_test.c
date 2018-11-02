#include <xinu.h>
#include <xmalloc.h>

void xmalloc_test() {
	// printf("start testing the xmalloc...\n");

	// initialize segregated memory allocation
	xmalloc_init();
	
	// allocate ramdon size of buffers
	srand(97);
	int32 ntest = 64 + 32 + 16;
	char **bufptr = (char **)getmem(ntest * sizeof(char *));
	int32 i;
	for (i = 0; i < ntest; i++) {
		int32 bpmaxb;
		if (i < 64) {
			bpmaxb = 128 - sizeof(int32);
		} else if (i < 96) {
			bpmaxb = 1024 - sizeof(int32);
		} else {
			bpmaxb = BP_MAXB - sizeof(int32);
		}
		int32 size = (int32)(rand() + BP_MINB) % bpmaxb;
		printf("=====================================================\n");
		printf("allocation #%d, buffer size: %d\n", i, size);
		char *addr = (char *)xmalloc(size);
		if (addr == NULL) {
			printf("xmalloc failed, size: %d\n", size);
		}
		*(bufptr + sizeof(char *) * i) = addr;
	}

	for (i = 0; i < ntest; i++) {
		printf("=====================================================\n");
		printf("free #%d, address: %d\n", i, *(bufptr + sizeof(char *) * i));
		char *ptr = *(bufptr + sizeof(char *) * i);
		if (ptr == NULL) {
			printf("buffer did not allocated!\n");
			continue;
		}
		xfree((void *)ptr);
	}

	char *str = xheap_snapshot();
	printf("Information about the fragmentation in heap: \n");
	printf("%s", str);
	
	// printf("this is the end of testing.\n");
}