#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

static int alarm_fired = 0;

void ding (int sig) {
	alarm_fired = 1;
}

int main () {
	pid_t pid;
	printf ("Alarm app start.\n");	

	pid = fork ();
	switch (pid) {
		case -1:
			// failure
			perror ("Fork failed");
			exit (1);
		case 0:
			// child
			sleep (5);
			kill (getppid(), SIGALRM); // send SIGALRM to parrent process
			exit (0);
	}
	// from here is parent process
	printf ("waiting for alarm to go off\n");
	(void) signal (SIGALRM, ding);

	pause (); // causes the program to suspend execution until a signal occurs
	if (alarm_fired)
		printf("Ding!\n");;

	printf("done\n");;
	exit (0);
}