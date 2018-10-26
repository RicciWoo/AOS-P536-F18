#include <xinu.h>
#include <future.h>
#include <stdio.h>
#include <stdlib.h>


uint future_prod(future_t *fut, int n) {
    printf("Produced %d\n", n);
    printf("before future_set, tail: %d\n", fut->get_queue->tail);
    future_set(fut, n);
    printf("after future_set, tail: %d\n", fut->get_queue->tail);
    return OK;
}

uint future_cons(future_t *fut) {
    int i, status;
    printf("before future_get, tail: %d\n", fut->get_queue->tail);
    status = (int)future_get(fut, &i);
    printf("after future_get, tail: %d\n", fut->get_queue->tail);
    if (status < 1) {
        printf("future_get failed\n");
        return -1;
    }
    printf("Consumed %d\n", i);
    return OK;
}
