#ifndef _FUTURE_H_
#define _FUTURE_H_  

#define HPID -1
#define TPID -2

struct qnode {
    pid32 pid;
    struct qnode *next;
    struct qnode *prev;
};

struct queue {
    struct qnode *head;
    struct qnode *tail;
};

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
    struct queue *set_queue;
    struct queue *get_queue;
} future_t;

/* Interface for the Futures system calls */
future_t *future_alloc(future_mode_t mode);
syscall future_free(future_t *);
syscall future_get(future_t *, int *);
syscall future_set(future_t *, int);
 
uint future_prod(future_t*, int);
uint future_cons(future_t*);

struct queue *initial_queue();
int is_empty(struct queue *);
void fenqueue(struct queue *, pid32);
pid32 fdequeue(struct queue *);

#endif /* _FUTURE_H_ */