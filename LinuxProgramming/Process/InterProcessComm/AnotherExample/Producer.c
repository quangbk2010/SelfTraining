#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// libraries to use shared memory
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
// library to get pid of process
#include <sys/types.h>
// libraries to get priority of process
#include <sys/time.h>
#include <sys/resource.h>

int  main(int argc, char const *argv[])
{
	printf("Producer Process start!!!!!\n");
	// the size (in bytes) of shared memory object
	const int SIZE = 4096;

	// name of the shared memory object
	const char *name = "OS";

	// strings written to shared memory
	const char *message_0 = "Hello";
	const char *message_1 = "World!";

	// get pid
	pid_t pid, ppid;
	pid = getpid();
	ppid = getppid();
	//printf("Producer process: %d : %d\n", pid, ppid);

	// get priority
	int priority;
	int which = PRIO_PROCESS;
	priority = getpriority (which, pid);
	printf("Producer process: pid: %d : prio: %d\n", pid, priority);

	// shared memory file descriptor
	int shm_fd;

	// pointer to shared memory object
	void *ptr;

	// create the shared memory object
	shm_fd = shm_open (name, O_CREAT | O_RDWR, 0666);

	// Configure the size of the shared memory object
	ftruncate (shm_fd, SIZE);

	// memory map to the shared memory object
	ptr = mmap (0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	// write to the shared memory object
	sprintf (ptr, "%s", message_0);
	ptr += strlen (message_0);
	sprintf (ptr, "%s", message_1);
	ptr += strlen (message_1);

	system ("cc -g -o Consumer Consumer.c -lrt");
	system ("./Consumer");
	return 0;
}