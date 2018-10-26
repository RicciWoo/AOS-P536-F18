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

struct qnode *newNode(pid32 pid) {
    struct qnode *new_node = (struct qnode *)getmem(sizeof(struct qnode));
    new_node->pid = pid;
    return new_node;
}

void freeNode(struct qnode *node) {
    freemem((char *)node, (uint32)sizeof(struct qnode));
}

struct queue *initial_queue() {
    struct queue *q = (struct queue *)getmem(sizeof(struct queue));
    q->head = NULL;
    q->tail = NULL;
    return q;
}

int is_empty(struct queue *q) {
    if (q->head == NULL) {
        return 1;
    } else {
        return 0;
    }
}

void fenqueue(struct queue *q, pid32 pid) {
    struct qnode *node = newNode(pid);
    node->next = NULL;
    node->prev = q->tail;
    if (q->tail != NULL) {
        q->tail->next = node;
    }
    q->tail = node;
    if (q->head == NULL) {
        q->head = node;
    }
}

pid32 fdequeue(struct queue *q) {
    pid32 pid = q->head->pid;
    struct qnode *temp = q->head;
    q->head = q->head->next;
    if (q->head != NULL) {
        q->head->prev = NULL;
    }
    if (q->head == NULL) {
        q->tail = NULL;
    }
    freeNode(temp);
    return pid;
}
