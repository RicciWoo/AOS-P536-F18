#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {

    // check argument of "--help"
    if (argc == 2 && strncmp(argv[1], "--help", 7) == 0) {
        printf("Usage: %s arg\n", argv[0]);
        printf("Ex arg: date, uname, hostname, echo\n");
        return 0;
    }

    // Check argument count
    if (argc < 2) {
        fprintf(stderr, "%s: too few argument\n", argv[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", argv[0]);
        return 1;
    }
    if (argc > 2) {
        fprintf(stderr, "%s: too many arguments\n", argv[0]);
        fprintf(stderr, "Try '%s --help' for more information\n", argv[0]);
        return 1;
    }

    // create a pipe
    int filedes[2];
    if (pipe(filedes) == -1) {
        fprintf(stderr, "Pipe Failed!\n");
        return 1;
    }

    // create 2 processes
    int pid = fork();

    if (pid < 0) {
        fprintf(stderr, "Fork error!\n");
        exit(1);
    } else if (pid > 0) {
        // In Parent Process, print pid
        printf("In Parent Process\n");
        printf("Parent PID: %d, Child Pid: %d\n", getpid(), pid);

        // write a string argument to pipe
        write(filedes[1], argv[1], strlen(argv[1]) + 1);

        // wait for child to execute
        wait(NULL);

    } else {
        // In Child Process, print pid
        printf("In Child Process\n");
        printf("PID of child is %d\n", getpid());

        // read the string argument from pipe
        char argStr[100];
        read(filedes[0], argStr, 100);

        // execute program corresponding to the argument
        char *comStr = basename(argStr);
        // printf("The command: %s\n", comStr);
        if (strncmp(comStr, "echo", 5) != 0)
            execlp(comStr, comStr, NULL);
        else
            execlp(comStr, comStr, "Hello World!", NULL);

    }
    return 0;  // executed by both parent and child 
}