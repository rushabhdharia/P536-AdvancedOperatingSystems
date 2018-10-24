#include<xinu.h>
#include<segmem.h>

shellcmd xmalloc_test(int nargs, char *args[])
{	
	//char *print;
	void* a = xmalloc(32);
	void* b = xmalloc(21);
	xheap_snapshot();
	xfree(a);
	xfree(b);
	printf("--------------------------------------------\n");
	xheap_snapshot();

	return 0;
	
}
