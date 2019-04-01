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
int main()
{
	printf("Consumer Process start!!!!!\n");
	/* the size (in bytes) of shared memory object */
	const int SIZE = 4096;
	/* name of the shared memory object */
	const char *name = "OS";

	// get pid
	pid_t pid, ppid;
	pid = getpid();
	ppid = getppid();
	//printf("Consumer process: %d :  %d\n", pid, ppid);

	// get priority
	int which = PRIO_PROCESS;
	int priority;
	priority = getpriority (which, pid);
	printf("Consumer process: pid: %d : prio: %d\n", pid, priority);

	// set priority
	int ret;
	int priority_value = 10;
	ret = setpriority (which, pid, priority_value);
	priority = getpriority (which, pid);
	printf("Consumer process: pid: %d : new prio: %d - %d\n", pid, ret, priority);

	/* shared memory file descriptor */
	int shm_fd;
	
	/* pointer to shared memory obect */
	void *ptr;

	/* open the shared memory object */
	shm_fd = shm_open(name, O_RDONLY, 0666);

	/* memory map the shared memory object */
	ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

	/* read from the shared memory object */
	printf("Consumer Process: %s\n",(char *)ptr);

	/* remove the shared memory object */
	shm_unlink(name);
	return 0;
}