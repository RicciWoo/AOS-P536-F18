#include <xinu.h>
#include <future.h>

// The Futures Interface

// Allocates a new future (in the FUTURE_EMPTY state) with the given mode.
// We will use the getmem() call to allocate space to new future.
future_t *future_alloc(future_mode_t mode){
    intmask mask = disable();
    future_t *myfuture;
    char *addr = getmem(sizeof(future_t));
    if (addr != (char *)SYSERR) {
        myfuture = (future_t *)addr;
        myfuture->state = FUTURE_EMPTY;
        myfuture->mode = mode;
        myfuture->set_queue = initial_queue();
        myfuture->get_queue = initial_queue();
        restore(mask);
        return myfuture;
    } else {
        restore(mask);
        return NULL;
    }
}

// Frees the allocated future. Use the freemem() system call to free the space.
syscall future_free(future_t *f){
    freemem((char *)f, (uint32)sizeof(future_t));
    return OK;
}

// Get the value of a future set by an operation and may change the state of future.
syscall future_get(future_t *f, int *value){
    intmask mask = disable();
    if (f->mode == FUTURE_EXCLUSIVE) {
        if (f->state == FUTURE_EMPTY) {
            f->pid = getpid(); // save pid of the future_get process
            f->state = FUTURE_WAITING;
            suspend(f->pid); // suspend this process, wait for future_set
        }
        if (f->state == FUTURE_READY) {
            *value = f->value; // get value when this process get resumed
            f->state = FUTURE_EMPTY;
            restore(mask);
            return OK;
        }
        // only allow one future_get process to wait for the future
        // already exist one, not allow others to wait for the same future
        restore(mask);
        return SYSERR;
    } else if (f->mode == FUTURE_SHARED) {
        if (f->state != FUTURE_READY) {
            f->state = FUTURE_WAITING;
            pid32 pid = getpid();
            fenqueue(f->get_queue, pid); // enqueue the process
            suspend(pid); // suspend the process
        }
        *value = f->value; // get value when the process get resumed
        if (is_empty(f->get_queue) == 1) {
            f->state = FUTURE_EMPTY;
        }
        restore(mask);
        return OK;
    } else if (f->mode == FUTURE_QUEUE) {
        if (is_empty(f->set_queue) == 0) {  // the set_queue is not empty
            pid32 pid = fdequeue(f->set_queue);  // dequeue one from set_queue
            resume(pid);  // then resume it
        }
        // only happens when not be resumed, enqueue this process
        // when dequeue and resumed from get_queue, state should be READY
        // and should not enquue again
        if (f->state == FUTURE_EMPTY) {
            pid32 pid = getpid();
            fenqueue(f->get_queue, pid);
            suspend(pid);
        }
        *value = f->value;  // get value after resumed
        f->state = FUTURE_EMPTY;
        restore(mask);
        return OK;
    } else {    
        restore(mask);
        return SYSERR;
    }
}

// Set a value in a future and may change the state of future.
syscall future_set(future_t* f, int value){
    intmask mask = disable();
    if (f->mode == FUTURE_EXCLUSIVE) {
        if (f->state == FUTURE_EMPTY) { // no process is waiting
            f->value = value;  // set value
            f->state = FUTURE_READY;
            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_WAITING) { // one process is waiting
            f->value = value;  // set value
            f->state = FUTURE_READY;
            resume(f->pid);  // resume the only one waiting process
            restore(mask);
            return OK;
        }
        // only allow one future_set process to access the future
        // already exists one, not allows others try to set the same future
        restore(mask);
        return SYSERR;
    } else if (f->mode == FUTURE_SHARED) {
        if (f->state != FUTURE_READY) { // state is empty or waiting
            f->value = value; // set value
            f->state = FUTURE_READY;
            // resume all processes in get_queue if exist
            while (is_empty(f->get_queue) == 0) {
                pid32 pid = fdequeue(f->get_queue);
                resume(pid);
            }
            restore(mask);
            return OK;
        }
        // only allow one future_set process to access the future
        // already exists one, not allows others try to set the same future
        restore(mask);
        return SYSERR;
    } else if (f->mode == FUTURE_QUEUE) {
        if (is_empty(f->get_queue) == 1) { // the get_queue is empty
            pid32 pid = getpid();
            fenqueue(f->set_queue, pid); // enqueue this process to set_queue
            suspend(pid); // suspend itself
        }
        // get_queue is not empty or this process is resumed
        f->value = value; // set value
        f->state = FUTURE_READY;             // after set value, 
        pid32 pid = fdequeue(f->get_queue);  // dequeue one from get_queue
        resume(pid); // then resume it
        restore(mask);
        return OK;
    } else {   
        restore(mask);
        return SYSERR;
    }
}

qnode_t *newNode(pid32 pid) {
    qnode_t *new_node = (qnode_t *)getmem(sizeof(qnode_t));
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
}

pid32 fdequeue(queue_t *q) {
    qnode_t *node = q->head;
    pid32 pid = node->pid;
    q->head = node->next;
    if (q->head != NULL) {
        qnode_t *temp = q->head;
        temp->prev = NULL;
    }
    if (q->head == NULL) {
        q->tail = NULL;
    }
    freeNode(node);
    return pid;
}
