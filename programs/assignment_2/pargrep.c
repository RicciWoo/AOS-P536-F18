#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <cctype>
#include <vector>

using namespace std;

#define MAX_NUM_THREADS 198

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
    long start;
    DataPass(int l, long s) : len(l), start(s) {}
};

// create vector of indexlines pointers
vector<IndexLines*> index_lines;
char *word;

void *searchWord(void *dp) {
    DataPass *data_pass = (DataPass*)dp;
    int len = data_pass->len;
    long start = data_pass->start;
    for (int i = 0; i < len; i++) {
        IndexLines *idx_line = index_lines[start + i];
        if (!strstr(idx_line->line, word)) {
            free(idx_line->line);
            idx_line->line = NULL;
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {

    FILE *fin;
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    // start the timer
    struct timespec start, stop;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

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
        printf("%s: too few argument\n", argv[0]);
        printf("Try '%s --help (-h)' for more information\n", argv[0]);
        return 1;
    }
    if (argc > 4) {
        printf("%s: too many arguments\n", argv[0]);
        printf("Try '%s --help (-h)' for more information\n", argv[0]);
        return 1;
    }

    // read lines from the file or standard input
    if (strncmp(argv[1], "-", 1) == 0) {
        // has specified # of threads, multi-threading mode
        if (argc == 2) {
            printf("%s: too few argument for multi threads\n", argv[0]);
            printf("Try '%s --help (-h)' for more information\n", argv[0]);
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
            printf("%s: number of threads must be 1 <= t <= %d\n", argv[0], MAX_NUM_THREADS);
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
        pthread_attr_t attr;
        int rc;
        int t;
        void *status;

        long num_lines = 0;
        // read from a file or standard input on multi-thread mode
        if (argc == 4) {
            // open a file
            fin = fopen(argv[3], "r");
            if (fin == NULL) {
                printf("%s: Cannot open the file %s\n", argv[0], argv[3]);
                return 2;
            }

            // read each line from a file
            while ((nread = getline(&line, &len, fin)) != -1) {
                IndexLines *temp_il = new IndexLines(num_lines++, line);
                index_lines.push_back(temp_il);
                line = NULL;
                len = 0;
            }

            // close the file
            fclose(fin);
        } else {
            // read from standard input (pipe)
            while ((nread = getline(&line, &len, stdin)) != -1) {
                IndexLines *temp_il = new IndexLines(num_lines++, line);
                index_lines.push_back(temp_il);
                line = NULL;
                len = 0;
            }
        }

        // calculate number of lines for each thread
        long lines_per_thread = num_lines / num_threads;
        if (num_lines % num_threads != 0) lines_per_thread++;
        long lines_last_thread = num_lines - 
                                 lines_per_thread * (num_threads - 1);

        // set the data_pass for passing to function in thread
        DataPass *data_pass = (DataPass*)malloc(sizeof(DataPass) * num_threads);
        for (int i = 0; i < num_threads; i++) {
            data_pass[i].len = lines_per_thread;
            data_pass[i].start = lines_per_thread * i;
        }
        // update for the last thread
        data_pass[num_threads - 1].len = lines_last_thread;

        // initialize and set thread detached attribute
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        // set the word argument for function in thread
        word = argv[2];

        // create multiple threads
        for (t = 0; t < num_threads; t++) {
            rc = pthread_create(&threads[t], &attr, searchWord, (void*)&data_pass[t]);
            if (rc) {
                printf("%s: return code from pthread_create() is %d", argv[0], rc);
                return 3;
            }
        }

        // join the threads
        pthread_attr_destroy(&attr);
        for (t = 0; t < num_threads; t++) {
            rc = pthread_join(threads[t], &status);
            if (rc) {
                printf("%s: return code from pthread_join() is %d\n", argv[0], rc);
                return 3;
            }
        }

        // print results, and then free the lines
        for (int i = 0; i < index_lines.size(); i++) {
            if (index_lines[i]->line != NULL) {
                printf("%s", index_lines[i]->line);
                free(index_lines[i]->line);
                index_lines[i]->line = NULL;
            }
        }

        // let threads that created to finish execution
        //pthread_exit(NULL);

    } else {
        // normal mode (with single thread)
        if (!checkChar(argv[1])) {
            printf("%s: Invalid input on search word\n", argv[0]);
            return 1;
        }

        if (argc == 4) {
            printf("%s: too many arguments for single thread\n", argv[0]);
            printf("Try '%s --help (-h)' for more information\n", argv[0]);
            return 1;
        }

        if (argc == 3) {
            // open the file
            fin = fopen(argv[2], "r");
            if (fin == NULL) {
                printf("%s: Cannot open the file %s\n", argv[0], argv[2]);
                return 2;
            }

            // read from the file
            while ((nread = getline(&line, &len, fin)) != -1) {
                if (strstr(line, argv[1])) {
                    printf("%s", line);
                }
            }

            // close the file
            fclose(fin);
        } else {
            // read from standard input (pipe)
            while ((nread = getline(&line, &len, stdin)) != -1) {
                if (strstr(line, argv[1])) {
                    printf("%s", line);
                }
            }            
        }

        // free the line pointer
        if (line) free(line);
    }

    // stop the timer and print runtime
    clock_gettime(CLOCK_MONOTONIC_RAW, &stop);
    long long delta = (stop.tv_sec - start.tv_sec) * 1000000 + 
                     (stop.tv_nsec - start.tv_nsec) / 1000;
    printf("\nRuntime: %ld\n", delta);

    return 0;
}