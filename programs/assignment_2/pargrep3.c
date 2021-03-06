#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <math.h>

typedef struct str_thdata
{
	char *wordToFind;
	FILE *fileinput;
	int th_no;
	int counter;
	char **buffer;
	int size;
} thdata;

void *Find(void *ptr)
{
	thdata *data;            
    data = (thdata *) ptr; 
    char *word = data->wordToFind;
    FILE *fileinput = data->fileinput;
    char *buffer = *data->buffer;
    int no = data->th_no;
    int counter = data->counter;
    int size = data->size;
    int counter1;
    int newCount = 0;
    int i;

    ssize_t read;
	size_t len = 0;
	char *line;
	line = NULL;
	line = malloc(200*sizeof(char));

	counter1 = counter*no;
	for(i=0;i<counter1;i++)
		read = getline(&line, &len, fileinput);

    while ((read = getline(&line, &len, fileinput)) != -1)
    {
    	if(strstr(line, word))
    		strcat(buffer, line);
    	newCount++;
    	if(newCount==counter)
    		break;
    }
    free(line);
    
    pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int n;						//number of threads
	char *word;					//word to find	
	char *filename; 			// filename
	filename = NULL;
	word = NULL;
	word = malloc(20*sizeof(char));
	filename=malloc(50*sizeof(char));
	char **buffer;

	if (argc==2)
	{
		n = 5;
		strcpy(filename, "newfile.txt");
		strcpy(word, argv[1]); 
	}
	else if(argc == 3)
	{
		n = 5;
		strcpy(word, argv[1]);
		strcpy(filename, argv[2]);
	}
	else
	{
		n = atoi(argv[1]);
		strcpy(word, argv[2]);
		strcpy(filename, argv[3]);
	}
	FILE *fp;
	FILE *inputFiles[n];
			
	pthread_t threads[n];		// declaring n threads
	thdata th[n];				// stuct for each thread
	int rc, t, count;
	
	int size = 0;
	double counter_d = 0;
	int counter = 0;
	int newCount;			
    char c;  					// To store a character read from file 

    ssize_t read;
	size_t len = 0;

	if(argc==2)
	{
		 fp = fopen(filename, "w+");
    	if (fp == NULL)
		{
			printf("Could not open file %s", argv[3]); 
			exit(EXIT_FAILURE);
		}

		while (EOF != (c = fgetc(stdin))) 
		{
         	putc(c, fp);
        }
		fseek(fp, 0, SEEK_SET);
	}
	else
	{
    	fp = fopen(filename, "r");
    	if (fp == NULL)
		{
			printf("Could not open file %s", argv[3]); 
			exit(EXIT_FAILURE);
		}
	}

	for (c = getc(fp); c != EOF; c = getc(fp)) 
	{	
		size+=1;
        if (c == '\n') 			// Increment count if this character is newline 
            counter_d = counter_d + 1; 
    }

    size=2*size/n;
    counter_d = counter_d/n;
	counter = (int) ceil(counter_d);
	fseek(fp, 0, SEEK_SET);

	buffer = NULL;
	buffer = malloc(n * sizeof(char *));
	for (t = 0; t < n; t++)
	{
	   inputFiles[t]=fopen(filename, "r");
	   buffer[t] = NULL;
	   buffer[t] = malloc(size*sizeof(char));
	}

	for(t = 0; t<n; t++)
	{
		th[t].wordToFind = word;
		th[t].fileinput = inputFiles[t];
		th[t].buffer = &buffer[t];
		th[t].th_no = t;
		th[t].counter = counter;
		th[t].size = size;
	}

	for(t=0; t<n; t++)
	{
		rc = pthread_create(&threads[t], NULL, (void *) &Find, (void *) &th[t]);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}	

	for(t=0; t<n; t++)
		pthread_join(threads[t], NULL);
	
	for(t=0;t<n;t++)
	{
		printf("%s", buffer[t]);
	}

	fclose(fp);
	for(t=0;t<n;t++)
	{
		fclose(inputFiles[t]);
		free(buffer[t]);
	}
	if(argc==2)
	{
		remove(filename);	
	}
	free(buffer);
	free(filename);
	free(word);
	pthread_exit(NULL);
  	return 0;
}