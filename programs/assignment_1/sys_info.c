/* sys_info.c sys_info  */

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int pid;
	int p[2];
	char path[20];	
	int a = strlen(argv[1]);
	//printf("%d\n",a);
	//printf("%d\n",strlen("/bin/date"));
	
	if (pipe(p) == -1)
	{
		fprintf(stderr, "Pipe Failed, Error no = %d \n", errno);
		return 1;
	}

	pid = fork();

	if(pid == -1) // Fork command failed
	{
		fprintf(stderr, "Error no = %d\n", errno); // man7.org/linux/man-pages/man3/errno.3.html
		return 1;
	}
	
	else if(pid == 0) // Child process
	{
		printf("Child PID = %d\n", getpid());
		close(p[1]);
		read(p[0], path, sizeof(path));
		printf("%d \t",a);
		printf("%s \n", path);
		execl(path, "", 0);
		close(p[0]);
	}
	
	else // Parent process
	{
		printf("Parent PID = %d\n", getpid());
		close(p[0]);
		//printf("%s\n", argv[1]);
		write(p[1], argv[1], a+1);
		close(p[1]);
		wait(NULL); //Wait for child
	}	

	return 0;
}


