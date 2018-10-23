#ifndef _SEGMEM_H_
#define _SEGMEM_H_

#define MAX_HEAP_SIZE 2560

typedef struct {
	int poolsize;
	int free; // 0 - not free, 1 - free
	int frag_size;
}pool_info;


void xmalloc_init();
void* xmalloc(int);
void xfree(void*);
void xheap_snapshot();

//extern void *base_ptr = sbrk((int)MAX_HEAP_SIZE)

#endif
