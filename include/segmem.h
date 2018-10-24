#ifndef _SEGMEM_H_
#define _SEGMEM_H_

#define MAX_HEAP_SIZE 2560

struct node
{
	int pointer;
	int size;
	struct node *next;
};

void xmalloc_init();
void* xmalloc(int);
void xfree(void*);
void xheap_snapshot();

struct node *createll();
struct node *addNode(struct node*, int pointer, int size);
struct node *remove(struct node*, int pointer);

#endif
