#include<xinu.h>
#include<segmem.h>

bpid32 bp_a[5];

void xmalloc_init()
{
	int i,buff_size =32, num_of_buf = 32;
	//bpid32 bp_a[5];
	bufinit();
	for(i = 0; i<4; i++)
	{
		bp_a[i] = mkbufpool(buff_size,num_of_buf);
		buff_size*=2;
		num_of_buf /= 2;
	}
	for(i=0;i<4;i++)
	{
		printf("%d",(int)bp_a[i]);
	}
	
}

void* xmalloc(size_t size)
{
	char *ptr;
	if(size<32)
	{
		ptr = getbuf(0);	
	}
	else if(size<64)
	{
		ptr = getbuf(1);
	}
	else if(size<128)
	{
		ptr =
	}
}

void xfree(void* ptr)
{


}
