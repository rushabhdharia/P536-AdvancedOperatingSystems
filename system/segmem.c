#include<xinu.h>
#include<segmem.h>

bpid32 bp_a[6];
int alloc_buff[6];
int alloc_bytes[6];
int pointer_id[65];
int size_buf [65];
int associated_pool[65];
int count = 0;

void xmalloc_init()
{
	int i,buff_size =32, num_of_buf = 32;
	/*
 	buffer id | buffer_size | Number of buffers in pool
 	0 - 32 x 32
	1 - 64 x 16
	2 - 128 x 8
	3 - 256 x 4
	4 - 512 x 2
	5 - 1024 x 1
	*/
	bufinit();
	for(i = 0; i<=5; i++)
	{
		bp_a[i] = mkbufpool(buff_size,num_of_buf);
		alloc_buff[i] = 0;
		buff_size*=2;
		num_of_buf /= 2;
	}
}

void* xmalloc(int size)
{
	char *ptr;
	int i;
	if(size<=32)
		i=0;	
	else if(size<=64)
		i=1;
	else if(size<=128)
		i=2;
	else if(size<=256)
		i=3;
	else if(size <= 512)
		i=4;
	else if(size <=1024)
		i=5;
	else
		i=-1;

	if(i == -1) 
	{
		printf("Cannot allocate memory more than 1024 bytes.");
		return (void*)-1;
	}
	else
	{
		ptr = getbuf((bpid32)i);
		//printf("%d\n",(int)ptr);
		alloc_buff[i]+=1;
		alloc_bytes[i]+=size;	
		pointer_id[count]=(int)ptr;
		size_buf[count] = size;
		associated_pool[count]= i;
		count+=1; 
	}
	return (void*) ptr;
}

void xfree(void* ptr)
{
	int i,j;
	char *p = (char*)ptr;
	int condition = (int) ptr;
	//for(i=0;i<2;i++)
	//{
	//	printf("%d\t", pointer_id[i]);
	//}
	//printf("\ncondition = %d", condition);
	for(i=0;i<65;i++)
	{
		if(pointer_id[i]==condition)
		{	
			//printf("\nHere\n");
			j = associated_pool[i];
			alloc_buff[j] -= 1;
			alloc_bytes[j] -= size_buf[i];
			break;
		}
	}
	freebuf(p);
	//if(freebuf(p)==SYSERR)
	//	printf("Cannot free\n");
	//else
	//	printf("Freed\n");
}

void xheap_snapshot()
{
	int i;
	//char* toprint;
	int buff_size=32, total_buff = 32, frag_bytes = 0;
	for(i=0;i<5;i++)
	{
		frag_bytes = alloc_buff[i]*buff_size-alloc_bytes[i];
		printf("pool_id=%d, buffer_size=%d, total_buffers=%d, allocated_bytes=%d, allocated_buffers=%d, fragmented_bytes=%d\n", i, buff_size, total_buff, alloc_bytes[i], alloc_buff[i], frag_bytes);
		buff_size*=2;		
		total_buff/=2;
	}
	//return toprint;
}
