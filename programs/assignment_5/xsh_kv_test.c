// copy of shell/xsh_kv_test.c
// in shell/shell.c, add: {"kv_test", FALSE, xsh_kv_test}, 
// in include/shprototypes.h, add: extern shellcmd xsh_kv_test (int32, char *[]);
#include <xinu.h>
#include <kv.h>


shellcmd xsh_kv_test(int nargs, char *args[]) {

	if (nargs >= 3 && strncmp(args[2], "test", 20) == 0) {

		resume( create(kv_test, 1024, 20, "kv_test", 0) );
		return 0;

	} else if (nargs >= 3 && strncmp(args[2], "small", 20) == 0) {

		resume( create(trace_small, 1024, 20, "trace_small", 0) );
		return 0;

	} else if (nargs >= 3 && strncmp(args[2], "long", 20) == 0) {

		resume( create(trace_long, 1024, 20, "trace_long", 0) );
		return 0;

	} else if (nargs >= 3 && strncmp(args[2], "big", 20) == 0) {

		resume( create(trace_big, 1024, 20, "trace_big", 0) );
		return 0;

	} else if (nargs >= 3 && strncmp(args[2], "large", 20) == 0) {

		resume( create(trace_large, 1024, 20, "trace_large", 0) );
		return 0;

	} else if (nargs >= 3 && strncmp(args[2], "xlarge", 20) == 0) {

		resume( create(trace_xlarge, 1024, 20, "trace_xlarge", 0) );
		return 0;

	}

	fprintf(stderr, "%s: incorrect  arguments\n", args[0]);
	fprintf(stderr, "Try '%s -f' with options test, small, big, long, large, xlarge\n",args[0]);
	return 1;
}
