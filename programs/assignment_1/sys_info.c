/* sys_info.c sys_info  */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int pid;
	int p[2];	//p[0] - read end of the pipe, p[1] - write end of the pipe
	char path[20];	//to store the path
	
        if(!argv[1]) 	//checks if argument is passed.
	{
		printf("Please enter a path. For example = /bin/date\n");
		return 1;		
	}
	if (pipe(p) == -1)	//checks if pipe is created 
	{
		fprintf(stderr, "Pipe Failed, Error no = %d \n", errno);
		return 1;
	}

	pid = fork();

	if(pid == -1) 	// Fork command failed
	{
		fprintf(stderr, "Error no = %d\n", errno); 	// man7.org/linux/man-pages/man3/errno.3.html
		return 1;
	}
	
	else if(pid == 0) 	// Child process
	{
		printf("Child PID = %d\n", getpid());
		close(p[1]); 	//close write end of pipe as it is not required
		read(p[0], path, sizeof(path));
		if(strcmp(path,"/bin/echo")==0)
		{
			execl(path, "-c", "\"Hello World!\"", "", 0); 	//https://stackoverflow.com/questions/32771494/how-can-i-use-execl-function-or-other-kinds-of-exec-functions-to-perform-ech/
			fprintf(stderr, "Error no = %d\n", errno);
		}
		else
		{
			execl(path, "", 0);
			perror("Error: "); 
			fprintf(stderr, "Error no = %d\n", errno);
		}
		close(p[0]); 	//close read end
	}
	
	else // Parent process
	{
		printf("Parent PID = %d\n", getpid());
		close(p[0]); //close read end of pipe as it is not required
		write(p[1], argv[1], strlen(argv[1]));
		close(p[1]); //close write end
		wait(NULL); //Wait for child
	}	

	return 0;
}


