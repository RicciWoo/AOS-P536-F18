// this is a copy of shell/xsh_prodcons.c
// in shell/shell.c, add: {"prodcons", FALSE, xsh_prodcons}, 
// in include/shprototypes.h, add: extern shellcmd xsh_prodcons (int32, char *[]);
#include <xinu.h>
#include <stdio.h>
#include <string.h>
#include <future.h>

shellcmd xsh_prodcons(int nargs, char *args[]) {

	future_t *f_exclusive, 
	         *f_shared, 
	         *f_queue;
	  
	f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
	f_shared    = future_alloc(FUTURE_SHARED);
	f_queue     = future_alloc(FUTURE_QUEUE);

	if (strncmp(args[2], "e", 20) == 0) {
		// Test FUTURE_EXCLUSIVE
		// resume( create(future_prod, 1024, 20, "fprod9", 2, f_exclusive, 9) );
		resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
		// resume( create(future_cons, 1024, 20, "fcons0", 1, f_exclusive) );
		resume( create(future_prod, 1024, 20, "fprod1", 2, f_exclusive, 1) );
		// resume( create(future_prod, 1024, 20, "fprod9", 2, f_exclusive, 9) );
		return 0;
	} else if(strncmp(args[2], "s", 20) == 0){
		// Test FUTURE_SHARED
		// resume( create(future_prod, 1024, 20, "fprod8", 2, f_shared, 8) );
		resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
		resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
		resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
		resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
		resume( create(future_prod, 1024, 20, "fprod2", 2, f_shared, 2) );
		// resume( create(future_prod, 1024, 20, "fprod8", 2, f_shared, 8) );
		return 0;
	} else if(strncmp(args[2], "q", 20) == 0){
		// Test FUTURE_QUEUE
		// resume( create(future_prod, 1024, 20, "fprod7", 2, f_queue, 7) );
		resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
		resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
		resume( create(future_cons, 1024, 20, "fcons8", 1, f_queue) );
		resume( create(future_cons, 1024, 20, "fcons9", 1, f_queue) );
		resume( create(future_prod, 1024, 20, "fprod3", 2, f_queue, 3) );
		resume( create(future_prod, 1024, 20, "fprod4", 2, f_queue, 4) );
		resume( create(future_prod, 1024, 20, "fprod5", 2, f_queue, 5) );
		resume( create(future_prod, 1024, 20, "fprod6", 2, f_queue, 6) );
		// resume( create(future_prod, 1024, 20, "fprod7", 2, f_queue, 7) );
		return 0;
	}
	
	fprintf(stderr, "%s: incorrect  arguments\n", args[0]);
	fprintf(stderr, "Try '%s -f' with flags e, s, q\n",args[0]);
	return 1;
}
