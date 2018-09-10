/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * xsh_hello - say hello to someone
 *------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {

	/* Output info for '--help' argument */

	if (nargs == 2 && strncmp(args[1], "--help", 7) == 0) {
		printf("Usage: %s <string>\n", args[0]);
		printf("Description:\n");
		printf("\tSay hello to someone.\n");
		return 0;
	}

	/* Check argument count */

	if (nargs < 2) {
		fprintf(stderr, "%s: too few argument\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return 1;
	}

	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return 1;
	}

	if (nargs == 2) {
		printf("Hello %s\n", args[1]);
		return 1;
	}

	return 0;
}
