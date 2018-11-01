#include <xinu.h>
#include <xmalloc.h>

shellcmd xsh_xmalloc_test(int nargs, char *args[]) {

	resume( create(xmalloc_test, 1024, 20, "xmalloc_test", 0) );
	
	return 1;
}