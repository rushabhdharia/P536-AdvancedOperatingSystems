#include<xinu.h>
#include<segmem.h>

shellcmd xmalloc_test(int nargs, char *args[])
{	
	//char *print;
	xmalloc_init();
	void* a = xmalloc(22);
	void* b = xmalloc(21);
	xheap_snapshot();
	xfree(a);
	xfree(b);
	
	void* c = xmalloc(511);
	void* d = xmalloc(128); 
	printf("\n--------------------------------------------\n");
	xheap_snapshot();
	
	void* e = xmalloc(67);
	void* f = xmalloc(38);
	printf("\n-----------------------------------------------\n");
	xheap_snapshot();
	xfree(c);
	xfree(d);
	xfree(e);
	xfree(f);
	printf("\n-----------------------------------------------\n");
	xheap_snapshot();
	
	return 0;
	
}
