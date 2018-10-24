#ifndef _SEGMEM_H_
#define _SEGMEM_H_

void xmalloc_init();
void* xmalloc(int);
void xfree(void*);
void xheap_snapshot();

#endif
