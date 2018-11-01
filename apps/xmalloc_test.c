#include <xinu.h>
#include <xmalloc.h>

void xmalloc_test() {
	printf("start testing the xmalloc...\n");
	xmalloc_init();
	
	srand(97);
	uint32 ntest = 32;
	char **bufptr = (char **)getmem(ntest * sizeof(char *));
	uint32 i;
	// allocate ramdon size of buffers
	for (i = 0; i < ntest; i++) {
		uint32 size = (uint32)(rand() + BP_MINB) % BP_MAXB;
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
	printf("the return string: %s\n", str);
	printf("this is the end of testing.\n");
}