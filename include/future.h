#ifndef _FUTURE_H_
#define _FUTURE_H_  

#define HPID -1
#define TPID -2

typedef struct {
    pid32 pid;
    qnode_t *next;
    qnode_t *prev;
} qnode_t;

typedef struct {
    qnode_t *head;
    qnode_t *tail;
} queue_t;

typedef enum {
    FUTURE_EMPTY,
    FUTURE_WAITING,
    FUTURE_READY
} future_state_t;

typedef enum {
    FUTURE_EXCLUSIVE,
    FUTURE_SHARED,
    FUTURE_QUEUE
} future_mode_t;

typedef struct {
    int value;
    future_state_t state;
    future_mode_t mode;
    pid32 pid;
    queue_t *set_queue;
    queue_t *get_queue;
} future_t;

/* Interface for the Futures system calls */
future_t *future_alloc(future_mode_t mode);
syscall future_free(future_t *);
syscall future_get(future_t *, int *);
syscall future_set(future_t *, int);
 
uint future_prod(future_t*, int);
uint future_cons(future_t*);

queue_t *initial_queue();
int is_empty(queue_t *);
void fenqueue(queue_t *, pid32);
pid32 fdequeue(queue_t *);

#endif /* _FUTURE_H_ */