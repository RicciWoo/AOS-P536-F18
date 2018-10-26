#include <xinu.h>
#include <future.h>

// The Futures Interface

// Allocates a new future (in the FUTURE_EMPTY state) with the given mode.
// We will use the getmem() call to allocate space to new future.
future_t *future_alloc(future_mode_t mode){
    intmask mask = disable();
    future_t *myfuture;
    char *addr = getmem(sizeof(myfuture));
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
        if (f->state == FUTURE_READY) {
            *value = f->value;
            f->state = FUTURE_EMPTY;
            restore(mask);
            return OK;
        }
        // the state of the future is EMPTY or WAIT
        f->state = FUTURE_WAITING;
        pid32 pid = getpid();
        fenqueue(f->get_queue, pid); // enqueue the process
        suspend(pid); // suspend the process
        *value = f->value; // get value when the process get resumed
        //if (is_empty(f->get_queue) == 1) {
            f->state = FUTURE_EMPTY;
        //}
        restore(mask);
        return OK;
    } else if (f->mode == FUTURE_QUEUE) {
        pid32 pid = getpid();
        if (is_empty(f->set_queue) == 1) {
            fenqueue(f->get_queue, pid);
            printf("pid %d", pid);
            suspend(pid);
            *value = f->value;
            restore(mask);
            return OK;
        } else {
            pid = fdequeue(f->set_queue);
            resume(pid);
            restore(mask);
            return OK;
        }
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
            f->state = FUTURE_READY;
            f->value = value;
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
        pid32 pid=getpid();
        if (is_empty(f->get_queue) == 1) {
            fenqueue(f->set_queue, pid);
            suspend(pid);
            f->value = value;
            restore(mask);
            return OK;
        } else {
            pid = fdequeue(f->get_queue);
            resume(pid);
            restore(mask);
            return OK;
        }
    } else {   
        restore(mask);
        return SYSERR;
    }
}
