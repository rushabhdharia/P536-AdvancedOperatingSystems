#include<xinu.h>
#include<segmem.h>

shellcmd xmalloc_test(int nargs, char *args[])
{	
	//a = (int *) xmalloc(sizeof(int));
	//b = (char *) xmalloc(sizeof(char)*10);
	void* a = xmalloc(32);
	printf("2 = %d\n", &a);
	//void* b = xmalloc(21);
	xheap_snapshot();
	xfree(a);
	//xfree(b);
	printf("--------------------------------------------\n");
	xheap_snapshot();

	return 0;
	
}
