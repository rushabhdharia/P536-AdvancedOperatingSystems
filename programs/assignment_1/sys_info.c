/* sys_info.c sys_info  */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#define BUFFER_SIZE = 20

int main(int argc, char *argv[])
{
	int pid;
	int p[1];
	char *command = NULL;	
	char buffer[BUFFER_SIZE]

	pid = fork();
	pipe(x);

	if(pid == -1) // Fork command failed
	{
		fprintf(stderr, "Error no = %d\n", errno); // man7.org/linux/man-pages/man3/errno.3.html
		return 1;
	}
	
	else if(pid == 0) // Child process
	{
		printf("Child PID = %d\n", getpid());
		read(p[0], buffer, BUFFERSIZE);
	}
	
	else // Parent process
	{
		printf("Parent PID = %d\n", getpid());
		//pipe(x);
		
		//close(x[0]);
		write(p[1], argv[1], sizeof(argv[1]));
			
		wait(NULL); //Wait for child
		close(x);

	}	

	return 0;
}


