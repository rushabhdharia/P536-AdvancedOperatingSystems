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
int counter = 0;

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

	if(counter<100)
		counter++;
	printf("%d\n", counter);
	printf("%s\n", key);
	printf("%s\n", value);
	temp->key = key;
	temp->value = value;
	temp->next = NULL;
	//printf("%d\n", counter);
	printf("%s\n", temp->key);
	printf("%s\n", temp->value);

	if(counter == 100)
	{
		if(!kv_delete(head->key))
			return 1;
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
}
