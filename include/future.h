#ifndef _FUTURE_H_
#define _FUTURE_H_

struct node{
  pid32 pid;
  struct node *next;
  struct node *prev;
};

struct Queue{
	struct node *front;
	struct node *rear;
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
  struct Queue *set_queue;
  struct Queue *get_queue;
} future_t;

extern sid32 sem_future;
extern sid32 set;
extern sid32 get;

/* Interface for the Futures system calls */
future_t* future_alloc(future_mode_t mode);
syscall future_free(future_t*);
syscall future_get(future_t*, int*);
syscall future_set(future_t*, int);

uint future_prod(future_t*, int);
uint future_cons(future_t*);

struct node* newNode(pid32);
struct Queue* createQueue();
int is_empty(struct Queue*);

void enqueue_future(struct Queue*, pid32);
pid32 dequeue_future(struct Queue*);

 
#endif /* _FUTURE_H_ */
