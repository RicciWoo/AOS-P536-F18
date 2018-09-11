#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    // check argument of "--help", or "-h"
    if (argc == 2 && (strncmp(argv[1], "--help", 7) == 0) ||
                     (strncmp(argv[1], "-h", 3) == 0)) {
        printf("Usage: %s [-t] word [file]\n", argv[0]);
        printf("Search for <word> in provided <file> or standard input.\n");
        printf("%s options:\n", argv[0]);
        printf("-t          number of threads\n");
        printf("word        string to be searched in the file");
        printf("file        file to lookup or search");
        return 0;
    }

    // Check argument count
    if (argc < 2) {
        fprintf(stderr, "%s: too few argument\n", argv[0]);
        fprintf(stderr, "Try '%s --help (-h)' for more information\n", argv[0]);
        return 1;
    }
    if (argc > 4) {
        fprintf(stderr, "%s: too many arguments\n", argv[0]);
        fprintf(stderr, "Try '%s --help (-h)' for more information\n", argv[0]);
        return 1;
    }

    // read lines from the file or standard input
    if (strncmp(argv[1], "-", 1) == 0) {
        char *sub = malloc()
        // has specified # of threads, multi-threading mode

    } else {
        // normal mode (with single thread)
        FILE *fin;
        char *line = NULL;
        size_t len = 0;
        ssize_t nread;

        if (argc == 3) {
            // read from a file
            fin = fopen(argv[2], "r");
            if (fin == NULL) {
                fprintf(stderr, "Cannot open the file %s\n", argv[2]);
                return 2;
            }
            while ((nread = getline(&line, &len, stream)) != -1) {
                if (strstr(line, argv[1])) {
                    printf("%s\n", line);
                }
            }
        }
    }


    return 0;
}