#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int funct_1 () {
	int a = 3;
	printf("a = %d\n", a);
	return a;
}
int main()
{
	pid_t pid;
	char *message, total_message[10];
	int n = 3;
	printf("fork program starting\n");
	int j = funct_1 ();
	//fprintf (stderr, "error\n");
	//setbuf (stdout, NULL);
	pid = fork();
	//printf("Pid = %d\n", pid);
	switch(pid){
		case -1:
			fprintf(stderr, "fork failed");
			exit(1);
		case 0:
			//n=3;
			message = "Child";
			sprintf (total_message, "Child: %d", j);
			break;
		default:
			//n=4;
			//waitpid (pid, NULL, 0);
			message = "Parent";
			sprintf (total_message, "Parent: %d", j);
			break;
	}
	for(; n>0 ; n--){
		printf("\n%s %d %d %d\n", total_message, n, getpid(), getppid());
		sleep(1);
	}
	return 0;
}

