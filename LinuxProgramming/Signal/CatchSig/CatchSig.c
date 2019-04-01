#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void catch (int sig) {
	printf ("Ohh, I got signal %d", sig);
	(void) signal (SIGINT, SIG_DFL);
}

int main () {
	(void) signal (SIGINT, catch);

	while (1) {
		printf("Hello world!\n");
		sleep (1);
	}
}