#include <xinu.h>
#include <xmalloc.h>
#include <kv.h>

int hashFunc(char *key) {
	if (key == NULL || *key == '\0') {
		return -1;
	}

	int hashCode = 0;
	for (int i = 0; i < MAX_KEY_SIZE; i++) {
		if (*key == '\0') {
			break;
		}

		hoshCode = (hashCode * 31 + *key) % MAX_KEY_NUMB;
		key++;
	}

	return hashCode;
}

void kv_test() {
	// initialize segregated memory allocation
	xmalloc_init();

	// allocate memory for key and value
	//int keyLen = strlen(key);
	//int valLen = strlen(val);

	char *keyPtr;
	keyPtr = xmalloc(64);
	memset((void *)keyPtr, 0, 64);

	strncat(keyPtr, "ctttjyhm\0", 9);

	hashFunc(keyPtr);
	//int hashedVal = hashFunc("rmxrodelucysiaytouifbtryfcdveypsjw");

	printf("hash code for key: %s\n", keyPtr);
	//printf("hash code for val: %d\n", hashedVal);

	xfree((void *)keyPtr);
}