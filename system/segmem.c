#include<xinu.h>
#include<segmem.h>

bpid32 bp_a[6];
int alloc_buff[6];
int alloc_bytes[6];
int pointer_id[65];
int size = [65];
int associated_pool[65];
int count = 0;

struct node *head0 = NULL;
struct node *head1 = NULL;
struct node *head2 = NULL;
struct node *head3 = NULL;
struct node *head4 = NULL;
struct node *head5 = NULL;
/*
//-----------------------------------------------------------------------
// Code Adapted from -  Data Structures using C - Reema Thareja page 175
struct node *createll()
{
	struct node *temp = (struct node*)getmem(sizeof(struct node*));
	temp->pointer=NULL;
	temp->size=NULL;
	temp->next=NULL;	
	return temp;
}

struct node *addNode(struct node* start, int pointer, int size)
{
	struct node *newnode = (struct node*)getmem(sizeof(struct node*));
	struct node *temp;
	//printf("AddedPointer1 = %d", pointer);
	newnode->pointer=pointer;
	newnode->size = size;
	temp = start;
	while(temp->next!=NULL)
		temp=temp->next;
	temp->next = newnode;
	newnode->next = NULL;
	//printf("\nAdded\n");
	printf("Added pointer2 = %d\n", pointer);
	return start;
}

struct node *remove(struct node* start, int pointer)
{
	struct node *preptr, *ptr;
	ptr = start;
	printf("Removing pointer = %d\n", pointer);

	if(ptr->pointer == pointer)
	{
		start=start->next;
		//freemem((char*)ptr,sizeof(ptr) );
	}
	else
	{
		while(ptr->pointer != pointer)
		{
			preptr = ptr;
			ptr = ptr->next;
		}
		preptr->next = ptr->next;
		//freemem((char*)ptr,sizeof(ptr));
	}
	return start;

}

void printll(struct node* start)
{
	struct node *ptr;
	ptr = start;
	printf("\n----------------------\n");
	while(ptr != NULL)
	{
		printf("/t%d", ptr->size);
		ptr = ptr->next;
	}
	printf("\n-------------------\n");
}

//----------------------------------------------------------------------------
*/
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
	//head0 = createll();
	//head1 = createll();
	//head2 = createll();
	//head3 = createll();
	//head4 = createll();
	//head5 = createll();
}

int decide(int size)
{
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
	return i;
}
/*
void removeFromList(int ptr)
{
	//int i=-1;
	printf("Removing pointer 3 = %d \n", &ptr);
	int pr =(int) &ptr;
	//printf("Removing pointer 4 = %d\n", &pr);
	//printf("pr = %d\n", pr);
	struct node *find;
	find = head0;
	do
	{
		if(find->pointer == pr)
		{
			printf("\nhere-------------\n");
			alloc_bytes[0]-=head0->size;
			remove(head0, pr);
			alloc_buff[0]-=1;
			break;
		}
		find = find->next;
	}while(find->next!=NULL);
	
}
*/
void* xmalloc(int size)
{
	char *ptr;
	//int pr;
	int i = decide(size);
	if(i == -1) 
	{
		printf("Cannot allocate memory more than 1024 bytes.");
	}
	else
	{
		ptr = getbuf(i);
		//pr = (int)ptr;	
		//printf("Adding Pointer %d\n", &ptr);
		//printf("Adding pr = %d\n",&pr);
		alloc_buff[i]+=1;
		alloc_bytes[i]+=size;
		
		//if(i==0)
		//	head0 = addNode(head0,pr,size);	
		//printll(head0);	
	}
	return (void*) ptr;
}

void xfree(void* ptr)
{
	//int ptr1=(int)ptr;
	//printf("Removing Pointer1 = %d\n",&ptr);
	//removeFromList(ptr1);
	//printf("Removing Pointer2.5 = %d\n", (int)&ptr);
	/*
	struct node *find;
	find = head0;
	do
	{
		if(find->pointer == (int)ptr)
		{
			printf("\nhere-------------\n");
			alloc_bytes[0]-=head0->size;
			remove(head0,(int) ptr);
			alloc_buff[0]-=1;
			break;
		}
		find = find->next;
	}while(find->next!=NULL);
	*/
	//removeFromList(&ptr);
	char *p = (char*)ptr;
	if(freebuf(p)==SYSERR)
		printf("not done\n");
	else
		printf("success\n");

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
