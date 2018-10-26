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
    freemem((char *)f, (uint32)sizeof(f));
    return OK;
}

// Get the value of a future set by an operation and may change the state of future.
syscall future_get(future_t *f, int *value){
    intmask mask = disable();
    if (f->mode == FUTURE_EXCLUSIVE) {
        if (f->state == FUTURE_EMPTY) {
            f->pid = getpid(); // save pid of the waiting process
            f->state = FUTURE_WAITING;
            suspend(f->pid); // suspend the only one process
        }
        if (f->state == FUTURE_READY) {
            *value = f->value; // get value when the process get resumed
            f->state = FUTURE_EMPTY;
            restore(mask);
            return OK;
        }
        // there exists one process waiting for the future
        // do not allow other process to wait for the future
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
        if (is_empty(f->set_queue) == 0) {
            pid32 pid = fdequeue(f->set_queue);
            resume(pid);
        } else {
            pid32 pid = getpid();
            fenqueue(f->get_queue, pid);
            suspend(pid);
        }
        // while (f->state != FUTURE_READY) {
        //     continue; // wait until f->state == FUTURE_READY
        // }
        *value = f->value;
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
        if (f->state == FUTURE_EMPTY) {
            f->value = value;
            f->state = FUTURE_READY;
            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_WAITING) {
            f->state = FUTURE_READY;
            f->value = value;
            resume(f->pid); // resume the only one waiting process
            restore(mask);
            return OK;
        }
        // the future is already set READY by some process
        // do not allow other process to set value in the future
        restore(mask);
        return SYSERR;
    } else if (f->mode == FUTURE_SHARED) {
        if (f->state == FUTURE_EMPTY) {
            f->value = value;
            f->state = FUTURE_READY;
            restore(mask);
            return OK;
        }
        if (f->state == FUTURE_WAITING) {
            f->value = value;
            f->state = FUTURE_READY;
            // resume all processes in get_queue
            while (is_empty(f->get_queue) == 0) {
                pid32 pid = fdequeue(f->get_queue);
                resume(pid);
            }
            restore(mask);
            return OK;
        }
        // the future is already set READY by some process
        // do not allow other process to set value in the future
        restore(mask);
        return SYSERR;
    } else if (f->mode == FUTURE_QUEUE) {
        // enqueue itself if state is ready or set_queue is empty
        if (f->state == FUTURE_READY || is_empty(f->set_queue) == 1) {
            pid32 pid = getpid();
            fenqueue(f->set_queue, pid);
            suspend(pid);
        }
        // set_queue is not empty or resumed by get process
        f->value = value;
        pid32 pid = fdequeue(f->get_queue);
        resume(pid);
        f->state = FUTURE_READY;
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
