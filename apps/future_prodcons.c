#include <xinu.h>
#include <future.h>
#include <stdio.h>
#include <stdlib.h>


uint future_prod(future_t *fut, int n) {
    printf("Produced %d\n", n);
    future_set(fut, n);
    return OK;
}

uint future_cons(future_t *fut) {
    int i, status;
    status = (int)future_get(fut, &i);
    if (status < 1) {
        printf("future_get failed\n");
        return -1;
    }
    printf("Consumed %d\n", i);
    return OK;
}

struct qnode *newNode(pid32 pid){
    struct qnode *new_node = (struct qnode *)getmem(sizeof(struct qnode));
    new_node->pid = pid;
    return new_node;
}

struct qnode *initial_queue(){
    struct qnode *head;
    struct qnode *tail;
    head = newNode((pid32)HPID);
    tail = newNode((pid32)TPID);
    head->qprev = NULL;
    tail->qnext = NULL;
    head->qnext = tail;
    tail->qprev = head;
    return head;
}

int is_empty(struct qnode *head){
    struct qnode *temp = head->qnext;
    if(temp->pid == TPID){
        return 1;
    }
    else{
        return 0;
    }
}

void fenqueue(struct qnode* head, pid32 pid){
    struct qnode *curr = newNode(pid);
    struct qnode *temp = head->qnext;
    head->qnext = curr;
    temp->qprev = curr;
    curr->qnext = temp;
    curr->qprev = head;
}

pid32 fdequeue(struct qnode* head){
    struct qnode *tail = head->qnext;
    while(tail->pid != TPID){
        tail = tail->qnext;
    }
    struct qnode *temp = tail->qprev;
    struct qnode *last = temp->qprev;
    last->qnext = tail;
    tail->qprev = last;
    return temp->pid;
}
