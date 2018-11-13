#include<xinu.h>
#include<stdbool.h>
#include<kv.h>
#include<segmem.h>

// Note - Linked List implementation is referred from - https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm

struct node
{
	char *key;
	char *value;
	//int *no_of_hits;
	struct node *next;
};

struct node *head = NULL;
//int counter = 0;
int total_hits = 0;
int total_acc = 0;
int total_set_succ = 0;
int cache_size = 0;
int num_keys = 0;
int total_evict = 0;

int kv_init()
{
 	xmalloc_init();	
	return 0;
}


char* kv_get(char* key)
{	
	char* value;
	struct node* current = head;
	struct node* prev = current;
	total_acc+=1;
	//printf("Here 1\n");
	//printf("key = %s\n", key);
	if(head == NULL)
	{
		//printf("returning here");
		return NULL;
	}	
	
	while(current->key!=key)
	{	
		if (current->next==NULL)
		{
			return NULL;
		}
		else
		{
			prev = current;
			current = current->next;
		}
		//printf("Here 2\n");
		//printf("%s\n", current->key);
		//printf("%s\n", key);
	}
	total_hits+=1;
	value = current->value;
	printf("%s", value);
	prev->next = current->next;
	while(prev->next!=NULL)
		prev = prev->next;
	prev->next = current;	
	current->next = NULL;
	return value;
}

int kv_set(char* key, char* value)
{
	struct node *temp = (struct node*)xmalloc(sizeof(struct node));
	struct node *ptr;
	if(strlen(key)>64 || strlen(value)>1024)
		return SYSERR;
	if(num_keys<100)
		num_keys+=1;
	//printf("%d\n", num_keys);
	printf("%s\n", key);
	printf("%s\n", value);
	temp->key = key;
	temp->value = value;
	temp->next = NULL;
	//printf("%d\n", counter);
	printf("%s\n", temp->key);
	printf("%s\n", temp->value);

	if(num_keys == 100)
	{
		if(!kv_delete(head->key))
			return 1;
		total_evict+=1;
	}
	if(head == NULL)
	{
		head = temp;
	}
	else
	{
		ptr = head;
		while(ptr->next != NULL)
		{
			ptr = ptr->next;
		}
		ptr->next = temp;
	}
	//printf("\nI am here\n");
	total_set_succ+=1;
	return 0;  
}

bool kv_delete(char* key)
{
	struct node* current = head;
	struct node* prev = NULL;
	
	if(head == NULL)
	{
		return false;
	}
	
	while(current->key != key)
	{
		if(current->next == NULL)
		{
			return false;
		}
		else
		{
			prev = current;
			current = current->next;
		}
	}
	if(current == head)
	{
		head = head->next;
	}
	else
	{
		prev->next = current->next;
	}
	xfree(current);
	return true;
}

void kv_reset()
{
	struct node* current = head;
	struct node* prev = NULL;
	while(current->next != NULL)
	{	
		prev = current;
		current = current->next;
		xfree(prev);
	}
	total_hits = 0;
	total_acc = 0;
	total_set_succ = 0;
	cache_size = 0;
	num_keys = 0;
	total_evict = 0;
}

int get_cache_info(char *kind)
{
	int len = strlen(kind);
	if(strncmp(kind, "total_hits", len)==0)
	{
		//printf("hit = %d", total_hits);
		return total_hits;
	}
	else if(strncmp(kind, "total_accesses", len)==0)
		return total_acc;
	else if(strncmp(kind, "total_set_success", len)==0)
		return total_set_succ;
	else if(strncmp(kind, "cache_size", len)==0)
	{
		cache_size = num_keys * sizeof(struct node);
		return  cache_size;
	}
	else if(strncmp(kind, "num_keys", len)==0)
		return num_keys;

	else if (strncmp(kind, "total_evictions", len)==0)
		return total_evict;	
	return 1;
}

char** most_popular_keys(int k)
{
	char** popular_keys = (char **)xmalloc(k*sizeof(char*));
	int i;
	struct node *ptr;
	ptr = head;
	for(i=0; i<num_keys-k;i++)
	{
		ptr = ptr->next;
	}
	for(i=k;i<0;i++)
	{
		popular_keys[i] = ptr->key;
		ptr = ptr->next; 
		if(ptr->next == NULL)
			break;

	}
	return popular_keys;
}
