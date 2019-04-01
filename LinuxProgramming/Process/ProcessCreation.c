#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main ()
{
	pid_t pid;
	printf("hello\n");
	/* fork a child process */
	pid = fork();
	printf("start: pid = %d\n", pid);
	if (pid < 0) {/* error occurred */
		printf("Fork Failed: pid = %d\n", pid);
		fprintf (stderr, "Fork Failed");
		exit(-1);
	}
	else if (pid == 0) {/* child process */
		printf("child process\n");
		execlp ("/bin/ls", "ls" , NULL);
	}
	else {/* parent process */
		printf ("parent process");
		/* parent will wait for the child to complete */
		wait (NULL);
	}
	return 0;
}