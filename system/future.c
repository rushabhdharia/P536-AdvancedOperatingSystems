#include <xinu.h>
#include <future.h>
#include<stdio.h>

// Creates a new node in the queue and returns it
struct node *newNode(pid32 pid){
	struct node *temp = (struct node *)getmem(sizeof(struct node));
	temp->pid = pid;
	temp->next = NULL;
	temp->prev = NULL;
	return temp;
}

// Creates a new queue and returns it.
struct Queue *createQueue(){
	struct Queue *q = (struct Queue *)getmem(sizeof(struct Queue));
	q->front=NULL;
	q->rear=NULL;
	return q;
}

// Adds a node to the queue
void enqueue_future(struct Queue *q, pid32 p){
	struct node *temp = newNode(p);
	if(q->rear==NULL)
	{
		q->front = q->rear = temp;
		suspend(p);
		return;
	}
	q->rear->next = temp;
	temp->prev = q->rear;
	q->rear = temp;
	suspend(p);
}

//removes the first node from the queue and returns it's pid
pid32 dequeue_future(struct Queue *q){
	if(q->front == NULL)
	{
		return NULL;
	}
	pid32 p = q->front->pid;
	//struct node *temp = q->front;
	q->front = q->front->next;
	if(q->front == NULL){
		q->rear = NULL;
	}
	else{
		q->front->prev = NULL;
	}
	return p;
}

//checks if the Queue is empty
int is_empty(struct Queue *q){
	if(q->front !=NULL)
	{
		return 1;
	}
	return 0;
}

// Future System Call to allocate memory for the created future and set it's state, pid, mode, set_queue and get_queue.
future_t* future_alloc(future_mode_t mode){
	intmask mask = disable();
	future_t* f=(void*)getmem(sizeof(future_t));
	f->value=(int)getmem(sizeof(int));
	f->state=FUTURE_EMPTY;
	f->pid=getpid();
	f->mode=mode;
	f->set_queue=createQueue();
	f->get_queue=createQueue();		
	restore(mask);
	return f;
}

//System Call to Free the memory which was used to allocate the future
syscall future_free(future_t* f){
	intmask mask =disable();
	int val = freemem((char *)f,sizeof(future_t));
	restore(mask);
	if(val!=SYSERR)
		return OK;
	else
		return SYSERR;
}

// System Call to get the Future Value set by the Producer Thread
syscall future_get(future_t* f, int* value){
	intmask mask;
	mask = disable();
	if(f->mode == FUTURE_EXCLUSIVE){
			while(f->state != FUTURE_READY)
			{
				f->pid = getpid();
				f->state = FUTURE_WAITING;
				suspend(f->pid);
				*value = f->value;
				restore(mask);
				return OK;
			}
			*value = f->value;
			restore(mask);
			return OK;
	}
	else if(f->mode == FUTURE_SHARED){

		if(f->state==FUTURE_EMPTY)
		{
			f->pid = getpid();
			f->state = FUTURE_WAITING;
			enqueue_future(f->get_queue, f->pid);
			restore(mask);
			suspend(f->pid);
			*value = f->value;
		}
		else if(f->state == FUTURE_WAITING)
		{
			f->pid = getpid();
			enqueue_future(f->get_queue, f->pid);
			restore(mask);
			suspend(f->pid);
			*value = f->value;
		}
		else if(f->state == FUTURE_READY)
		{
			*value = f->value;
			restore(mask);
		}
	}
	else if(f->mode == FUTURE_QUEUE)
	{
		pid32 pid =getpid();
		//Try 1
		/*	
		if(f->state == FUTURE_EMPTY)
		{
			f->state = FUTURE_WAITING;
			enqueue_future(f->get_queue, pid);
			suspend(pid);
			*value = f->value;
		}
		else if(f->state == FUTURE_WAITING)
		{
			enqueue_future(f->get_queue, pid);
			suspend(pid);
			*value= f->value;
		}
		else if(f->state == FUTURE_READY)
		{
			pid = dequeue_future(f->set_queue);
			resume(pid);
			*value = f->value;
		}
			
		*/
		//Try 2
		if(is_empty(f->set_queue))
		{
			enqueue_future(f->get_queue, pid);
			restore(mask);
			f->state = FUTURE_WAITING;
			suspend(pid);
			*value = f->value;
			f->state=FUTURE_EMPTY;
			return OK;
		}	
		else
		{
			wait(set);
			restore(mask);
			pid = dequeue_future(f->set_queue);
			signal(get);
			resume(pid);
			*value = f->value;
			f->state = FUTURE_EMPTY;	
			return OK;
		}
		
	}
	else{
		restore(mask);
		return SYSERR;
	}
	restore(mask);
	return OK;
}

//System Call to Set the value produced by the Producer thread so that it can be used by the consumer thread.
syscall future_set(future_t* f, int value){
	intmask mask;
	pid32 pid;
	mask = disable();
	if(f->mode == FUTURE_EXCLUSIVE)
	{
		while(f->state != FUTURE_READY)
		{
			f->value = value;
			f->state = FUTURE_READY;
			resume(f->pid);
		}
		restore(mask);
		return OK;
	}
	else if(f->mode == FUTURE_SHARED)
	{
		if(f->state ==FUTURE_EMPTY)
		{
			f->value = value;
			f->state = FUTURE_READY;
			restore(mask);
		}
		else if(f->state == FUTURE_WAITING)
		{
			f->value = value;
			f->state = FUTURE_READY;
			restore(mask);
			while(f->get_queue!=NULL)
			{
				pid = dequeue_future(f->get_queue);
				resume(pid);
			}
		}	
	}

	else if(f->mode == FUTURE_QUEUE)
	{
		pid = getpid();
	/*	
		if(f->state == FUTURE_EMPTY)
		{
			f->state = FUTURE_READY;
			f->pid = pid;
			f->value = value;
			enqueue_future(f->set_queue, pid);
			suspend(pid);
		}	
		else if(f->state == FUTURE_WAITING)
		{
			pid = dequeue_future(f->get_queue);
			f->value = value;
			resume(pid);
		}
		else if(f->state == FUTURE_READY)
		{
			f->pid = pid;
			enqueue_future(f->set_queue, pid);
			suspend(pid);
			f->value = value;
		}*/
	if(is_empty(f->get_queue))
		{
			restore(mask);
			enqueue_future(f->set_queue, pid);
			suspend(pid);
			f->value = value;
			f->state = FUTURE_READY;
			return OK;
		}
	else
		{
			wait(get);
			f->value = value;
			f->state = FUTURE_READY;
			restore(mask);
			pid = dequeue_future(f->get_queue);
			signal(set);
			resume(pid);
			return OK;
		}
	}
	else
	{
		restore(mask);
		return SYSERR;
	}
	restore(mask);
	return OK;
}
