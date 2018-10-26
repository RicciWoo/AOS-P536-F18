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

qnode_t *newNode(pid32 pid) {
    struct qnode *new_node = (struct qnode *)getmem(sizeof(qnode_t));
    new_node->pid = pid;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

void freeNode(qnode_t *node) {
    freemem((char *)node, (uint32)sizeof(qnode_t));
}

queue_t *initial_queue() {
    queue_t *q = (queue_t *)getmem(sizeof(queue_t));
    q->head = NULL;
    q->tail = NULL;
    return q;
}

int is_empty(queue_t *q) {
    if (q->head == NULL) {
        return 1;
    } else {
        return 0;
    }
}

void fenqueue(queue_t *q, pid32 pid) {
    printf("enqueue, pid: %d, head: %d, tail: %d\n", pid, q->head, q->tail);
    qnode_t *node = newNode(pid);
    node->next = NULL;
    node->prev = q->tail;
    if (q->tail != NULL) {
        qnode_t *temp = q->tail;
        temp->next = node;
    }
    q->tail = node;
    if (q->head == NULL) {
        q->head = node;
    }
    printf("enqueue, pid: %d, head: %d, tail: %d\n", pid, q->head, q->tail);
}

pid32 fdequeue(queue_t *q) {
    qnode_t *node = q->head;
    pid32 pid = node->pid;
    printf("dequeue, pid: %d, head: %d, tail: %d\n", pid, q->head, q->tail);
    q->head = node->next;
    if (q->head != NULL) {
        qnode_t *temp = q->head;
        temp->prev = NULL;
    }
    if (q->head == NULL) {
        q->tail = NULL;
    }
    freeNode(node);
    printf("dequeue, pid: %d, head: %d, tail: %d\n", pid, q->head, q->tail);
    return pid;
}
