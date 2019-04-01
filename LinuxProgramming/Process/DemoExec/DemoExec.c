#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main ()
{
	pid_t pid;
	char *message;
	int n;
	printf("fork program starting\n");
	pid = fork();

	/* fork a child process */
	pid = fork();
	switch(pid){
		case -1:
			fprintf(stderr, "fork failed");
			exit(1);
		case 0:
			n=3;
			message = "Child";
			execlp ("/bin/ls", "ls" , NULL) ;
			break;
		default:
			n=2;
			wait (NULL);
			message = "Parent";
			break;
	}
	for(; n>0 ; n--){
		printf("%s %d %d\n", message, pid, n);
		sleep(1);
	}
}