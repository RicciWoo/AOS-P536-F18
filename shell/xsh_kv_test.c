#include <xinu.h>

shellcmd xsh_kv_test(int nargs, char *args[]) {

	resume( create(kv_test, 1024, 20, "kv_test", 0) );

	return 1;
}