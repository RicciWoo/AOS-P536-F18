#include <xinu.h>
#include <future.h>
#include <stdio.h>
#include <stdlib.h>

uint future_prod(future_t *fut, int n) {
    // printf("Try Produce %d\n", n);
    printf("Produced %d\n", n);
    int status;
    status = future_set(fut, n);
    if (status == SYSERR) {
        printf("future_set %d failed\n", n);
        return SYSERR;
    }
    // printf("Produced %d\n", n);
    return OK;
}

uint future_cons(future_t *fut) {
    int i, status;
    status = (int)future_get(fut, &i);
    if (status == SYSERR) {
        printf("future_get failed\n");
        return SYSERR;
    }
    printf("Consumed %d\n", i);
    return OK;
}
