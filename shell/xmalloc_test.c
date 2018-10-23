#include<xinu.h>

shellcmd xmalloc_test(int nargs, char *args[])
{

	int *a;
	char *b;
	
	xmalloc_init();
	a = (int *) xmalloc(sizeof(int));
	b = (char *) xmalloc(sizeof(char)*10);
	//xheap_snapshot();
	//xfree(a);
	//xfree(b);
	///xheap_snapshot();

	return 0;
	
}
