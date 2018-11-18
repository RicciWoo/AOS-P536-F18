#include <xinu.h>
#include <kv.h>


shellcmd xsh_kv_test(int nargs, char *args[]) {

	resume( create(trace_small, 1024, 20, "trace_small", 0) );

	return 1;
}
