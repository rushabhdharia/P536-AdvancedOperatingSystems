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
	return 1;
}


char* kv_get(char* key)
{	
	char* value;
	int count1 = 0;
	printf("Here 1\n");
	printf("key = %s\n", key);
	struct node* current = head;
	if(head == NULL)
	{
		printf("returning here");
		return NULL;
	}	
	
	while(current->key!=key)
	{	
		count1++;
		if (current->next==NULL)
		{
			return NULL;
		}
		else
		{
			current = current->next;
		}
		printf("Here 2\n");
		printf("%s\n", current->key);
		printf("%s\n", key);
	}
	value = current->value;
	printf("%s", value);
	return value;
}

int kv_set(char* key, char* value)
{
	struct node *temp = (struct node*)xmalloc(sizeof(struct node));
	struct node *ptr;
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
	printf("\nI am here\n");
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
