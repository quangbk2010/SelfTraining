#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	// the size (in bytes) of shared memory object
	const int SIZE 4096;

	// name of the shared memory object
	const char *name = "OS";

	// strings written to shared memory
	const char *message_0 = "Hello";
	const char *message_1 = "World!";

	// shared memory file descriptor
	int shm_fd;

	// pointer to shared memory object
	void *ptr;

	// create the shared memory object
	shm_fd = shm_open (name, O_CREATE | ORDRW, 0666);

	// Configure the size of the shared memory object
	ftruncate (shm_fd, SIZE);

	// memory map to the shared memory object
	ptr = mmap (0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

	// write to the shared memory object
	sprintf (ptr, "%s", message_0);
	ptr += strlen (message_0);
	sprintf (ptr, "%s", message_1);
	ptr += strlen (message_1);
	return 0;
}