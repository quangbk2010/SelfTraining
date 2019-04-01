#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
// libraries to use shared memory
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <semaphore.h>

#define SIZE 4 // the size (in bytes) of shared memory object
#define INPUTFILENAME "/media/sf_Humax_Computer/HumaxTraining/LinuxProgramming/Assignment/Produc_custom_communication.InputFile.txt"
#define OUTPUTFILENAME "/media/sf_Humax_Computer/HumaxTraining/LinuxProgramming/Assignment/Produc_custom_communication.OutputFile.txt"

/* Global variables */
pthread_mutex_t mutex;
FILE *ptr_infile; // pointer to input file
FILE *ptr_outfile; // pointer to output file
void *ptr_share1, *ptr_share2; // pointer to shared memory object
int shm_fd;	// shared memory file descriptor
int length = 0; // check size of data in shared memory

/* Functions used by customer, producer read/write items from/into a file */
void *write (void *ptr_msg);
void *read (void *ptr_msg);
int eof_or_line_break(char c);

/* main function */
int main () {
	pthread_t producer_thread, customer_thread;

	char *msg1 = "Producer thread", *msg2 = "Customer thread";

	// name of the shared memory object 
	const char *name = "OS";

	// create the shared memory object
	shm_fd = shm_open (name, O_CREAT | O_RDWR, 0666);
	// Configure the size of the shared memory object
	ftruncate (shm_fd, SIZE);

	// Create thread for Producer
	pthread_create (&producer_thread, NULL, write, (void *) msg1);
	// Create thread for Customer
	pthread_create (&customer_thread, NULL, read, (void *) msg2);
	
	// Producer thread wait
	pthread_join (producer_thread, NULL);
	// Customer thread wait
	pthread_join (customer_thread, NULL);

	// remove the shared memory object 
	shm_unlink(name);
	return 0;
}

// Check break line or EOF
int eof_or_line_break(char c) { 
    return (c == EOF);// == '\n' || 
}
/* Producer read data from Inputfile, write to shared buffer */
void *write (void *ptr_msg) {

	char *msg = (char *) ptr_msg;
	char buf[1000], c, line[100];
	int i = 0, j = 0, n, k, t = 0, k_max, temp = 0;

	printf("%s\n", msg);

	// Read data from Inputfile
	printf("Prod: Read data from Inputfile\n");
	ptr_infile = fopen ("InputFile.txt", "r");
	if (ptr_infile == NULL) {
		perror ("Prod: Cannot read input file!");
	}
	
	printf("Proc: begin reading\n");
	while (!eof_or_line_break(c = fgetc(ptr_infile)) ) {
		line[i ++] = c;
		printf("i = %c\n", c);

	}
	fclose (ptr_infile);

	n = i;
	// write data to shared memory
	printf("Prod: Write data to shared memory\n");
	while (1) {
		
		// number of characters which will be write to shared buffer in 1 time (used by producer):
		int no_char = 1 + random () % 3;
		k_max = no_char;

		if (length < SIZE && j < n) {
			// memory map to the shared memory object
			printf("\n\n***********\nProd: memory map to the shared memory object\tbuff length: %d\n", length);
			printf("Prod: want to add %d characters to shared memory\n", no_char);
			if ((SIZE - length) < no_char || j > n) {
				k_max = SIZE - length;
				printf("----Prod: we only add %d\n", k_max);
			}
			ptr_share1 = mmap (0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
			for (k = 1; k <= k_max; k ++) {
				
				sprintf (ptr_share1, "%c", line[j]);
				printf("Producer: s[%d] = %c\n", j, line[j]);
				//ptr_share1 += strlen (line);
				j += k;
				length += 1;
				t ++;
			}
			sleep (1);
		}
	}
	
}

/* Customer read and remove data from shared buffer, write to Outputfile */
void *read (void *ptr_msg) {

	char *msg = (char *) ptr_msg;
	char buf[1000];
	int k_max, k;

	printf("%s\n", msg);
	
	// open Outputfile to write
	printf("Cust: open Outputfile to write\n");
	
	// open and clear old data in output file (if not exist -> create) (overwrite)
	ptr_outfile = fopen ("OutputFile.txt", "w+");
	if (ptr_infile == NULL) {
		perror ("Cust: Cannot read output file!");
	}
	fclose (ptr_outfile);

	// read and remove data from shared buffer
	printf("Cust: Read and remove data from shared buffer\n");
	while (1) {
		// number of characters which will be write to shared buffer in 1 time (used by producer):
		int no_char = 1 + random () % SIZE;
		k_max = no_char;

		if (length > 0) {
			// open and write more in output file (append if file exist)
			ptr_outfile = fopen ("OutputFile.txt", "a");
			if (ptr_infile == NULL) {
				perror ("Cust: Cannot read output file!");
			}

			printf("Cust: Read data from shared buffer\t buff length: %d\n", length);

			// memory map to the shared memory object
			/*printf("\n\n***********\nCust: memory map to the shared memory object\tbuff length: %d\n", length);
			printf("Cust: want to read %d characters from shared memory\n", no_char);
			if (no_char > length) {
				k_max = length;
				printf("----Cust: we only read %d characters\n", k_max);
			}*/
			k_max = 1;
			for (k = 1; k <= k_max; k ++) {
				ptr_share2 = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
				fprintf(ptr_outfile,"%s\n", (char *)ptr_share2);
				printf("Customer: %s\n",  (char *)ptr_share2);
				//ptr_share2 -= strlen ((char *)ptr_share2);
				length --;
			}
			sleep (1);
			fclose (ptr_outfile);
		}
	}

}