#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <cctype>
#include <vector>

using namespace std;

#define LINES_EACH_READ 1024
#define MAX_NUM_THREADS 1024

int checkNumb(char *str) {
    size_t len = strlen(str);
    int i = 0;
    for (i = 0; i < len; i++)
        if (!isdigit(str[i])) return 0;
    return 1;
}

int checkChar(char *str) {
    size_t len = strlen(str);
    int i = 0;
    for (i = 0; i < len; i++)
        if (!isalpha(str[i])) return 0;
    return 1;
}

struct IndexLines {
    long index;
    char *line;
    IndexLines(long i, char *l) : index(i), line(l) {}
};

struct DataPass {
    int len;
    IndexLines *ptr;
    DataPass(int l, IndexLines *p) : len(l), ptr(p) {}
};

// create vector of indexlines pointers
vector<IndexLines*> index_lines;

// void *searchWord(void *) {
    
// }

int main(int argc, char* argv[]) {

    FILE *fin;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    // check argument of "--help", or "-h"
    if (argc == 2 && ((strncmp(argv[1], "--help", 7) == 0) ||
                     (strncmp(argv[1], "-h", 3) == 0))) {
        printf("Usage: %s [-t] word [file]\n", argv[0]);
        printf("Search for <word> in provided <file> or standard input.\n");
        printf("%s options:\n", argv[0]);
        printf("-t          number of threads\n");
        printf("word        string to be searched in the file\n");
        printf("file        file to lookup or search\n");
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
        // has specified # of threads, multi-threading mode
        if (argc == 2) {
            fprintf(stderr, "%s: too few argument for multi threads\n", argv[0]);
            fprintf(stderr, "Try '%s --help (-h)' for more information\n", argv[0]);
            return 1;
        }

        // get number of threads to be used
        size_t len = strlen(argv[1]);
        if (len <= 1) {
            printf("%s: Please specify number of threads\n", argv[0]);
            return 1;
        }
        char *str = (char *)malloc(len);
        strncpy(str, argv[1] + 1, len);

        // check if input number of thread is valid
        if (!checkNumb(str)) {
            printf("%s: Invalid input on number of threads\n", argv[0]);
            return 1;
        }

        // convert number of thread to int
        long num_threads_long = atol(str);
        if (num_threads_long > MAX_NUM_THREADS || num_threads_long < 1) {
            printf("%s: number of threads must be 1 <= t <= 1024\n", argv[0]);
            return 1;
        }
        int num_threads = (int)num_threads_long;

        // check if input search word is valid
        if (!checkChar(argv[2])) {
            printf("%s: Invalid input on search word\n", argv[0]);
            return 1;
        }

        // variable for creating threads
        pthread_t *threads = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
        int rc;

        long num_lines = 0;
        // read from a file or standard input on multi-thread mode
        if (argc == 4) {
            // read from a file， using multiple threads
            fin = fopen(argv[3], "r");
            if (fin == NULL) {
                fprintf(stderr, "%s: Cannot open the file %s\n", argv[0], argv[3]);
                return 2;
            }

            while ((nread = getline(&line, &len, fin)) != -1) {
                IndexLines *temp_il = new IndexLines(num_lines++, line);
                index_lines.push_back(temp_il);
                line = NULL;
                len = 0;
            }

            // calculate number of lines for each thread
            long lines_per_thread = num_lines / num_threads;
            if (num_lines % num_threads != 0) lines_per_thread++;
            long lines_last_thread = num_lines - 
                                     lines_per_thread * (num_threads - 1);

            // set the data_pass
            DataPass *data_pass = (DataPass*)malloc(sizeof(DataPass) * num_threads);
            for (int i = 0; i < num_threads; i++) {
                data_pass[i].len = lines_per_thread;
                data_pass[i].ptr = index_lines[lines_per_thread * i];
            }
            data_pass[num_threads - 1].len = lines_last_thread;

            printf("lines_per_thread: %ld\n", lines_per_thread);
            printf("lines_last_thread: %ld\n", lines_last_thread);

            for (int i = 0; i < num_threads; i++) {
                printf("thread #%d:\n", i);
                printf("length: %d, ", data_pass[i].len);
                printf("index: %ld\n", data_pass[i].ptr->index);
                printf("line: %s\n", data_pass[i].ptr->line);
            }

            // int t = 0;
            // for (t = 0; t < num_threads; t++) {
            //     rc = pthread_create(&threads[t], NULL, )
            // }

        } else {
            // read from standard input, using multiple threads

        }

        pthread_exit(NULL); // let threads that created to finish execution
        return 0;
    } else {
        // normal mode (with single thread)

        if (!checkChar(argv[1])) {
            printf("%s: Invalid input on search word\n", argv[0]);
            return 1;
        }

        if (argc == 3) {
            // read from a file
            fin = fopen(argv[2], "r");
            if (fin == NULL) {
                fprintf(stderr, "%s: Cannot open the file %s\n", argv[0], argv[2]);
                return 2;
            }

            while ((nread = getline(&line, &len, fin)) != -1) {
                if (strstr(line, argv[1])) {
                    printf("%s", line);
                }
            }

            fclose(fin);
            free(line);
            return 0;
        } else if (argc == 2) {
            // read from standard input
            while ((nread = getline(&line, &len, stdin)) != -1) {
                if (strstr(line, argv[1])) {
                    printf("%s", line);
                }
            }

            free(line);
            return 0;
        } else {
            fprintf(stderr, "%s: too many arguments for single thread\n", argv[0]);
            fprintf(stderr, "Try '%s --help (-h)' for more information\n", argv[0]);
            return 1;
        }
    }

    return 0;
}