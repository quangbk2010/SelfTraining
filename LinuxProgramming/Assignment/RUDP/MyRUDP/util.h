/*
*   - This header will define: 
*     + packet header, packet queue, transmission control block,
*     + several functions prototype which used to process packets
*   - This will be used in both client and server.
*/

#ifndef _UTIL_H
#define _UTIL_H

#define _BSD_SOURCE // That list is the pre-conditions for having usleep defined. It's basically a C-like expression involving #define variables which has to be true before including the header file.
/*
Note:
Those are called Include guards.

Once the header is included, it checks if a unique value (in this case HEADERFILE_H) is defined. Then if it's not defined, it defines it and continues to the rest of the page.

When the code is included again, the first ifndef fails, resulting in a blank file.

That prevent double declaration of any identifiers such as types, enums and static variables.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>   /* definitions for at least data types, exp. pthread_t */
#include <sys/socket.h>  /* Internet Protocol family, define some functions, macros relate to socket */
#include <sys/time.h>    /* defines the time structure */
#include <string.h>
#include <netdb.h>       /* definitions for network database operations, exp. structure: hostent, some types: in_addr */
#include <signal.h>
#include <math.h>

#include <setjmp.h>      /* definitions for sigsetjmp & siglongjmp */

/* Size of some structure */
#define HEADER_SIZE 	  16
#define PAYLOAD     	  495
#define PCK_SIZE          512
#define RCVBUFFER_BYTES   4096
#define RCVBUFFER         8    // 4096 / 495
#define SENDBUFFER        11   // buffer used to store packets which will be sent (the number of packet in sliding window will less than SENDBUFFER)  
#define WINSIZE 		  1    // start Sliding window of server
#define PROBE_SIZE        10
//#define MSS         1024 // Maximum segment size (unit: bytes)

/* Define type of packet */
#define SYN         1 // 1
#define SYN_ACK     2 // 2
#define SYN_ACK_ACK 3 // 3. These 3 parameters used in 3way handsake step
#define ACK         4
#define DATA        5
#define FIN         6 // 1
#define FIN_ACK     7 // 2. These 2 parameters used in termination step
#define PROBE       8 // 1
#define PROBE_ACK   9 // 2. These 2 parameters used for server: when rwnd = 0 => need to  probe
#define DUP_ACK     10 // Used in case of receiving duplicate acks

/* Time parameters with unit is ms */
#define TIMER_ACK    100
#define TIMER_FIN    100
#define TIMER_PROBE  100
#define RTT          30

/* Maximum of number retransmission, unless -> peer host is down */
#define RETRANS_MAX  10

/* lossrate */
#define LOSSRATE     0


/* This parameter is as a buffer used to store/save program location */
sigjmp_buf hello_bye_signal;
/* 
- Function sigsetjmp can be used to save the context of the processor (e.g. registers) 
and stack environment into a buffer. This context can then be retrieved using 
siglongjmp. The first time that sigsetjmp is called, it will return 0.
However, if it is returning from siglongjmp, it would return non-zero value.
- sigsetjmp saves the current location, CPU state and signal mask
- siglongjmp goes to the saved location, restoring the state and the signal mask.
*/

jmp_buf normal_buf; 


/* Packet structures, packet queue, TCB */

/* 1.
Packet structure:
________________
|Sequence Number| --- 4 bytes (Represents Sequence number of packet)
|_______________|
|Acknowledgement| ----4 bytes (Represents Ack number of packet)
|_______________|
|Window Size    | ----4 bytes (Represents Window Size)
|_______________|
| Flag          | ----4 bytes (Represents type of packet. SYN,ACK,DATA,PROBE,FIN,FIN_ACK,SYN_ACK)
|_______________|
|Payload        | ----496 bytes
|_______________| 
*/
typedef struct pckStruct_s
{
	uint32_t seqNum; // 4 bytes
	uint32_t ackNum; // 4 bytes    // at the moment, ackNum in server is not neccesary 
	uint32_t winSize; // 4 bytes
	uint32_t type; // 4 bytes
	char payload[PAYLOAD]; // size = 512 - 16 -1 = 495
} pckStruct_t;


/* 2.
Packet queue:

*/
typedef struct  pckQueue_s
{
	pckStruct_t packet;
	struct pckQueue_t *next;
	struct pckQueue_t *pre;

	int size; // size of ?

} pckQueue_t;


/* 3. 
Transmission Control Block:

	rwnd ----------------- allocation buffer by client and retransmission queue by server (rcvWindow)
    swnd------------------ allocation buffer by server and advertised window by client (sendWindow)

    base------------------Last unacknowledged sequence number at server (oldestSeqNum)
    ?------nextNewSeqNum---------last packet sent by server 
    nextSeqNum------------sequennce number to be sent out from server (nextSendSeqNum)
        
	seq------------------ sequence number printed 
    ack-------------------Ack no sent to server by client
    queueHead-------------Queue holding retransmission packet at server and received packets at client

*/
typedef struct tcb_s
{
	uint32_t rwnd;
	uint32_t swnd;

	uint32_t base;
	uint32_t nextSeqNum;

	uint32_t seq;
	uint32_t ack;
	pckQueue_t *queueHead; 	
} tcb_t;

/* Several function prototypes */
/* Note: items which should be changed => need to be defined as pointers. */
void print_packet (pckStruct_t packet);
int write_packet (pckStruct_t *packet, uint32_t seqNo, uint32_t ackNo, uint32_t win, uint32_t flag, char *data);   // Return the number of bytes write to the packet
int read_packet (pckStruct_t packet, uint32_t *seqNo, uint32_t *ackNo, uint32_t *win, uint32_t *flag, char *data); // Return the number of bytes read

//int send_packet (int sockfd, pckStruct_t *packet, char *msg); // Return the number of bytes to be sent
//int recv_packet (int sockfd, pckStruct_t *packet, char *msg); // Return the number of bytes to be reeceived.

void init_timer (struct itimerval *iTimer, int timeStart); //  timeStart: the time to the next timer expiration, unit: ms
/*
struct timeval {
  time_t tv_sec;   // seconds
  long tv_usec;    // microseconds
};
struct itimerval {
  struct timeval it_interval;    // timer interval: a period of time between events (pause) - reload the timer upon expiration
  								 // Setting it_interval to zero causes the timer to stop after the next expiration (assuming that it_value is non-zero).
  struct timeval it_value;       // current value: the time to the next timer expiration 
  								 // Setting it_value to zero disables a timer. 
};

int setitimer(int which, struct itimerval *value, struct itimerval *ovalue); // set value of interval timer

// Each process has two interval timers, ITIMER_REAL and ITIMER_PROF, which raise the signals SIGALRM and SIGPROF, respectively. 
// These are typically used to provide alarm and profiling capabilities.
*/
void stop_timer (struct itimerval *iTimer);

int is_packet_loss(int time_out, char *msg);//int sockfd, pckStruct_t *packet, int type, struct sockaddr *address, int addLen);
void time_out_handler (int *retransmitNo, char *peer_host, char *pck);

int sendUnreliably(int socket, pckStruct_t *packet, struct sockaddr *address, int addLen);

/*
* - get current time.
* - the result is returned in t_current.
*/
void current_time(struct timeval *t_current);

/*
* - obtains time duration between two time stamps.
* - Result in milisecond
*/
int get_durration(struct timeval *t_begin, struct timeval *t_end);

/* simulate RTT duration, we assume that packet receive immidiately after sending from server, 
   but due to RTT -> delay sending ACK by a duration equals RTT.
*/
void make_rtt (int ms);
#endif