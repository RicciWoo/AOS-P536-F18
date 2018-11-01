#include <xinu.h>
#include <xmalloc.h>

void xmalloc_test() {
	printf("start testing the xmalloc...\n");
	xmalloc_init();
	int32 *ptr;
	uint32 size;
	for (size = BP_MINB; size <= BP_MAXB; size += 8) {
		printf("=====================================================");
		printf("test for buffer size of %d", size);
		ptr = (int32 *)xmalloc(size);
	}
	xfree((void *)ptr);
	char *str = xheap_snapshot();
	printf("the return string: %s\n", str);
	printf("this is the end of testing.\n");
}