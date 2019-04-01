#ifndef _CLIENT_H
#define _CLIENT_H 

#include <setjmp.h>      /* definitions for sigsetjmp & siglongjmp */
#include <stdio.h>
#include <pthread.h>
#include "util.h"

#define CLIENT_INPUT 	   "client.in"
#define DOWLOADED_FILE     "Dld_file.txt"
#define MAXSIZE            20
#define IP_LEN             16
#define MAX_FILENAME_LEN   32
#define BUF_SIZE           (1024 * 1024)
#define CONSUM_RATE        20000//10000//56000 // bytes per second
#define HOST               "CLIENT"
#define PEER_HOST          "SERVER"

/* Arguments used to pass multiple params to the function which is called by threads */
typedef struct wr_argument_s
{
	FILE *fp;
	char *data;
} wr_argument_t;

typedef struct handl_argument_s
{
	int sockfd;
	struct sockaddr *addr;
	int addLen;
} handl_argument_t;


char serverIp[MAXSIZE];
int serverPort;
char dld_file_name [MAX_FILENAME_LEN];
int rwnd = RCVBUFFER, rwnd_byte;


int randomSeed;
float lossProbability;
float expDistributor;

int is_transfer_complete = 0;
int consumed_data; // bytes

pthread_mutex_t dataMutex = PTHREAD_MUTEX_INITIALIZER;

/* Show time */
struct timeval t_base, t_current;
double delta_t;

/* Parameter used to synchronise between handshake and data handler */
uint32_t synchr_flag = 0; // = 1 if receive data in handshake step



#endif