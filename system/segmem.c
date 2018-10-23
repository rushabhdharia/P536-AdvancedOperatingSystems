#include<xinu.h>
#include<segmem.h>

bpid32 bp_a[5];

void xmalloc_init()
{
	int i,buff_size =32, num_of_buf = 32;
	//bpid32 bp_a[5];
	/*
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
		buff_size*=2;
		num_of_buf /= 2;
	}
	for(i=0;i<=5;i++)
	{
		printf("%d",(int)bp_a[i]);
	}
	
}

void* xmalloc(int size)
{
	char *ptr;
	if(size<=32)
	{
		printf("32");
		ptr = getbuf(0);	
	}
	else if(size<=64)
	{
		printf("64");
		ptr = getbuf(1);
	}
	else if(size<=128)
	{	
		printf("128");
		ptr = getbuf(2);
	}
	else if(size<=256)
	{
		printf("256");
		ptr = getbuf(3);
	}
	else if(size <= 512)
	{
		printf("512");
		ptr = getbuf(4);
	}
	else if(size <=1024)
	{
		printf("1024");
		ptr = getbuf(5);
	}
	else 
	{
		printf("Cannot allocate memory more than 1024 bytes.");
	}
	
	return (void*) ptr;
}

void xfree(void* ptr)
{


}
