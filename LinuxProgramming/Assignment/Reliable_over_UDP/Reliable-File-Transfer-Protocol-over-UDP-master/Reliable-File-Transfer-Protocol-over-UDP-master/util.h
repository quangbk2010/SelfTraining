#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <signal.h>
#include <math.h>
#include "unp.h"
#include "unpifiplus.h"

#define port "62003"
#define HEADERSIZE 16
#define PAYLOAD 495
#define PCK_SIZE 512
#define DATAGRAM_LEN 512
#define HEADER_SIZE 16

#define SYN         1
#define SYN_ACK     2
#define SYN_ACK_ACK 3
#define ACK         4
#define DATA        5
#define FIN         6
#define FIN_ACK     7
#define PROBE       8
#define PROBE_ACK   9
#define DUP_ACKS    10

#define TIMER_CLIENT 3000
#define TIMER_FIN    3000
#define TIMER_PROBE  3000
#define RETRANS_MAX  12
 
typedef struct pckStruct pack_struct;
typedef struct pckStruct packStruct;

typedef struct client_Request_List {
    struct sockaddr_in cliaddr;
    struct client_Request_List *next;
    pid_t pid;
} clientRequestList;

struct pckStruct {
	uint32_t seqNum; // 4 bytes
	uint32_t ackNum; // 4 bytes
	uint32_t winSize; // 4 bytes
	uint32_t flag; // 4 bytes
	char payload[PAYLOAD]; // size = 512 - 16 - 1 = 495 bytes

};

typedef struct pckQueue {
	struct pckStruct packet;
	struct pckQueue *next;
	struct pckQueue *prev;

	int size;
	int present;
	int noOfRetransmits;
	uint32_t timestamp;
} pckQueue;

typedef struct{
	uint32_t rcvWindow;
	uint32_t sendWindow;
	uint32_t congWindow;
	uint32_t ssThresh;


	uint32_t nextNewSeqNum; //new seq no to be sent out for server 
	uint32_t nextSendSeqNum;//send seq number to be send out
	uint32_t oldestSeqNum; //Server, last packet in window
	
	uint32_t seq; // For client, it represets seq no printed
	uint32_t ack; // for client it represents pack no received
	struct	pckQueue *queueHead;
	struct pckQueue *queueCur; 
}tcb;

void updateTCB(tcb **head,struct pckStruct packet);
int print_interfaces(struct ifi_info *ifihead);
int is_local(struct ifi_info *ifihead,struct in_addr *iplocal,struct in_addr *ipremote);


void print_packet(pack_struct packet);
int writePacket(pack_struct *packet,uint32_t seqNo,uint32_t ackNo,uint32_t win,uint32_t flag,char *msg,int len);
int sendPacket(int fd,pack_struct *packet,int size,char *msg);
int recvPacket(int fd,pack_struct *packet,int size,char *msg);
int readPacket(pack_struct *packet,uint32_t len,uint32_t *seqNo,uint32_t *ackno,uint32_t *win,uint32_t *flag,char *
buf);
int initialize_timer(struct itimerval *itimer,long int time_start);
#endif

