#ifndef _FUTURE_H_
#define _FUTURE_H_  

#define HPID -1
#define TPID -2

struct qnode{
  pid32 pid;
  struct qnode *qnext;
  struct qnode *qprev;
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
  struct qnode* set_queue;
  struct qnode* get_queue;
} future_t;

extern sid32 ex;
extern sid32 set;
extern sid32 get;

/* Interface for the Futures system calls */
future_t* future_alloc(future_mode_t mode);
syscall future_free(future_t*);
syscall future_get(future_t*, int*);
syscall future_set(future_t*, int);
 
uint future_prod(future_t*, int);
uint future_cons(future_t*);

struct qnode* newNode(pid32);
struct qnode *initial_queue();
int is_empty(struct qnode*);
void fenqueue(struct qnode*, pid32);
pid32 fdequeue(struct qnode*);

#endif /* _FUTURE_H_ */