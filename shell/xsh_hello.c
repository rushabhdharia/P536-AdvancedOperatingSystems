/* xsh_hello.c - xsh_hello */

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------
 * xsh_hello.c - print a hello greeting
 *-----------------------------------------
 */

shellcmd xsh_hello(int nargs, char *args[])
{
	/* Check argument count */
	if (nargs == 2 && strncmp(args[1], "--help", 7) != 0)
	{
		printf("Hello %s, Welcome to the world of Xinu\n", args[1]);	/* used for greeting */
	}
	/*----------------------------------------------------------------------------------------------------------------*/
	/* Used the block of code provided in xsh_date.c from github.iu.edu/SICE-OS/P536-F18/blob/master/shell/xsh_date.c */

	else if(nargs > 2)
	{
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		fprintf(stderr, "Try '%s --help' for more information\n", args[0]);
		return 1;
	}
	
	else if(nargs < 2)
	{
		fprintf(stderr, "%s: too few arguments\n", args[0]);
		fprintf(stderr, "Try '%s' --help for more information\n", args[0]);
		return 1;
	}
	
	else
	{
		printf("Usage: %s\n\n", args[0]);
		printf("Description:\n");
		printf("\t Accepts a string and Displays Hello <string> message.\n");

	}

	 /*----------------------------------------------------------------------------------------------------------------*/

	return 0;
}
