#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
	pid_t pid;
	char *message;
	int n;
	int stat_val;
	printf("fork program starting\n");
	pid = fork();

	switch(pid){
		case -1:
			fprintf(stderr, "fork failed");
			exit(1);
		case 0:
			n=3;
			message = "Child";
			break;
		default:
			n=2;
			message = "Parent";
			break;
	}
	for(; n>0 ; n--){
		printf("%s %d %d\n", message, pid, n);
		sleep(1);
	}
	if(pid !=0){
		pid_t child_pid;
		child_pid = wait(&stat_val);
		printf("Child %d has finished\n", child_pid);
		if(WIFEXITED(stat_val))
			printf("Child exit with code %d\n", WEXITSTATUS(stat_val));
		else
			printf("Child terminate abnormally\n");
	}
	exit(WEXITSTATUS(stat_val));
}