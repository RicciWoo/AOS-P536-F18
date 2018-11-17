#include <xinu.h>
#include <kv.h>
#include <xmalloc.h>


void kv_test() {

	kv_init();

	kv_set("ctttjyhm", "rmxrodelucysiaytouifbtryfcdveypsjw");

	char *val = kv_get("ctttjyhm");

	printf("the value with key \"ctttjyhm\" is: %s\n", val);

	// // allocate memory for key and value
	// //int keyLen = strlen(key);
	// //int valLen = strlen(val);

	// char *keyPtr;
	// keyPtr = xmalloc(64);
	// memset((void *)keyPtr, 0, 64);

	// strncat(keyPtr, "ctttjyhm\0", 9);

	// int hashedKey = hashFunc(keyPtr);
	// //int hashedVal = hashFunc("rmxrodelucysiaytouifbtryfcdveypsjw");

	// printf("hash code for key: %d\n", hashedKey);
	// //printf("hash code for val: %d\n", hashedVal);

	// xfree((void *)keyPtr);
}
