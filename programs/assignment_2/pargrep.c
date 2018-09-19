#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <sys/stat.h>
#include <math.h>

typedef struct str_thdata
{
	char *wordToFind;
	char **buffer1;
	char **buffer2;
} thdata;

void *Find(void *ptr)
{
	thdata *data;            
    data = (thdata *) ptr; 
    char *word = data->wordToFind;
    char *buffer1 = *data->buffer1;
    char *buffer2 = *data->buffer2;
    char *line; 
    char *temp;
//https://stackoverflow.com/questions/35085702/reading-a-c-string-line-by-line--------
   line = strtok_r(buffer1, "\n", &temp);
   do {
   		if(strstr(line, word))
		   {
			 strcat(buffer2, line);
			 strcat(buffer2,"\n");
		   } 
     } while ((line = strtok_r(NULL, "\n", &temp)) != NULL);
//----------------------------------------------------------------------------------
	buffer2[strlen(buffer2)-1] = 0; //https://stackoverflow.com/questions/1726298/strip-first-and-last-character-from-c-string
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	FILE *fp;					//file pointer
	int n = atoi(argv[1]);		//number of threads
	char *word = argv[2];		//word to find	
	char *filename = argv[3]; 	// filename
	pthread_t threads[n];		// declaring n threads
	thdata th[n];				// stuct for each thread
	int rc, t, count;

	//https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
	struct stat st;				
	stat(filename, &st);		
	int size = st.st_size;	
	//----------------------------------------------------------------------------
	count = 2*size/n;
	char *buffer1[n];
	char *buffer2[n];
	double counter_d = 0;
	int counter = 0;
	int newCount;			
    char c;  					// To store a character read from file 

    ssize_t read;
	size_t len = 0;
	char *line = NULL;

    fp = fopen(filename, "r");
    if (fp == NULL)
	{
		printf("Could not open file %s", argv[3]); 
		exit(EXIT_FAILURE);
	}

	for (c = getc(fp); c != EOF; c = getc(fp)) 
        if (c == '\n') 			// Increment count if this character is newline 
            counter_d = counter_d + 1; 

    counter_d = counter_d/n;
	counter = (int) ceil(counter_d);

    fseek(fp, 0, SEEK_SET);

	for (t=0;t<n;t++)
	{
		buffer1[t] = NULL;
		buffer2[t] = NULL;
		buffer1[t] = (char *) malloc(count);
		buffer2[t] = (char *) malloc(count);
	}

	for(t=0;t<n;t++)
	{
		newCount = 0;
		
		while ((read = getline(&line, &len, fp)) != -1) //https://linux.die.net/man/3/getline
		{
			strcat(buffer1[t], line);
			newCount++;
			if(newCount==counter)
				break;
		}
		//printf("Buffer 1 After = %s", buffer1[t]);
	free(line);	
	}

	for(t=0; t<n; t++)
	{
		//printf("Buffer 1 = %s", buffer1[t]);
		//printf("Buffer 2 = %s", buffer2[t]);
		//memset(&buffer2[t][0], 0, sizeof(buffer2[t]));
		th[t].wordToFind = word;
		th[t].buffer1 = &buffer1[t];
		th[t].buffer2 = &buffer2[t];
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
		printf("%s\n", buffer2[t]);
	}

	fclose(fp);
	pthread_exit(NULL);
	return 0;
}
