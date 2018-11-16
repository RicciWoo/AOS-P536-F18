#include <xinu.h>
#include <kv.h>
#include <xmalloc.h>

void kv_test() {
	// initialize segregated memory allocation
	xmalloc_init();

	// allocate memory for key and value
	int keyLen = strlen(key);
	int valLen = strlen(val);

	int hashedKey = hashFunc("ctttjyhm");
	int hashedVal = hashFunc("rmxrodelucysiaytouifbtryfcdveypsjw");

	printf("hash code for key: %d\n", hashedKey);
	printf("hash code for val: %d\n", hashedVal);
}