/*
#include <stdio.h>
#include<xinu.h>
#include<segmem.h>
*/
/* 
1 - 32 x 32
2 - 64 x 16
3 - 128 x 4
4 - 256 x 2
5 - 512 x 1

Max heap size - 512*5 bytes
*/
//Reference https://stackoverflow.com/questions/25735905/implementing-segregated-memory-storage-malloc-in-c
/*
void *base_ptr = getmem((int)MAX_HEAP_SIZE);

struct pool_info
{
	int poolsize;
	int free; // 0 - not free, 1 - free
	int frag_size;	
};

void xmalloc_init()
{
	int i,j, n = 32, pool_size = 2;
	//void *base_ptr = sbrk((int)MAX_HEAP_SIZE);

	pool_info *header = (pool_info*)base_ptr;
	for(i=0;i<=4;i++)
	{
		for(j=0;j<n;j++)
		{
			header->poolsize = pool_size;
			header->free = 1;
			header->frag_size = 0;
			header += pool_size + sizeof(pool_info);		
		} 
		pool_size*=2;
		n /= 2;
	}

}
 
struct pool_info xmalloc(ssize_t size)
{
	pool_info *header = (pool_info*) base_ptr;
	while(header <=(pool_info*)base_ptr +(pool_info*) MAX_HEAP_SIZE)
	{
		if(header->poolsize >= size && header->free == 1)
		{
			header->free = 0;
			header->frag_size = size; 
			return (header + sizeof(header));
		}	
		header += header->poolsize + sizeof(pool_info);	
	}
	printf("Cannot allocate memory greater than 512 bytes");
	exit(0);
	//return (pool_info)SYSERR ;
}

void xfree(void *ptr)
{
	pool_info *header = (pool_info*)(ptr - sizeof(pool_info));
	header->free = 1;
}


void xheap_snapshot()
{
	int pool_id = 1;
	int buffer_size = 32;
	int total_buffers = 32;
	int allocated_bytes;
	int allocated_buffers;		
	int fragmented_bytes;
        int i,j;
	pool_info *header = (pool_info*)base_ptr;
	for(i=0; i<=4;i++)
	{
		allocated_bytes	= 0;
		allocated_buffers = 0;
		fragmented_bytes = 0;
		for(j=0;j<total_buffers;j++)
		{
			allocated_bytes+=header->frag_size;
			if(!header->free)
			{
				allocated_buffers=1;
			}
		}
		fragmented_bytes = total_buffers*buffer_size - allocated_bytes; 

		printf("pool_id=%d, buffer_size=%d, total_buffers=%d, allocated_bytes=%d, allocated_buffers=%d, fragemented_bytes=%d", &pool_id, &buffer_size, &total_buffers, &allocated_bytes, &allocated_buffers, &fragmented_bytes);		
	pool_id+=1;
	buffer_size*=2;
	total_buffers/=2;
	}


}
*/
