// copy of shell/xsh_xmalloc.c
// in shell/shell.c, add: {"xmalloc_test", FALSE, xsh_xmalloc_test}, 
// in include/shprototypes.h, add: extern shellcmd xsh_xmalloc_test (int32, char *[]);
#include <xinu.h>
#include <xmalloc.h>

shellcmd xsh_xmalloc_test(int nargs, char *args[]) {

	resume( create(xmalloc_test, 1024, 20, "xmalloc_test", 0) );
	
	return 1;
}
