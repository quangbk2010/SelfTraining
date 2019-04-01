#include "util.h"
#include "client.h"
#include <math.h>
#include "math.h"
#include <stdlib.h>
pthread_mutex_t tcbMutex = PTHREAD_MUTEX_INITIALIZER;
long int sleeptime;
int fin_flag;
//Aruments to be passed to thread
struct thread_arguments{
        tcb *clitcb;
        int *fd;
};

//Intializing client
int initilaize_client_tcb(tcb *clitcb,uint32_t cliwin,uint32_t servwin) {

        clitcb->nextSendSeqNum = 1;
        clitcb->ack = 1;
        clitcb->rcvWindow = max(cliwin,1);
        clitcb->sendWindow = max(cliwin,1);

        clitcb->seq = 0;
        clitcb->queueHead = (struct pckQueue *) calloc(cliwin,sizeof(struct pckQueue));

        return 1;
}

//Fast retransmit in case of missed packets
void fast_retransmit_acks(tcb *clitcb,int sockfd) {


	send_acks(clitcb,sockfd);
	send_acks(clitcb,sockfd);
	send_acks(clitcb,sockfd);
} 

//Send Acks mechanism
void send_acks(tcb *clitcb, int fd){
        pack_struct packet;
        uint32_t seqNo,ackNo,win;
        uint32_t flag;
        char buf[495] = "";
	int sockfd = fd;
        int len;

	seqNo = 1;
	ackNo = clitcb->ack;
	win = clitcb->sendWindow;
	if (win < 0)
		win = 0;

	flag = ACK;
	len = DATAGRAM_LEN;
	//print_packet(packet);
        writePacket(&packet,seqNo,ackNo,win,flag,buf,len);
        sendPacket(sockfd,&packet,len,"ACK");
	print_packet(packet);


}

//Sending Probes Reply
void send_probe_acks(tcb *clitcb,int fd) {
        pack_struct packet;
        uint32_t seqNo,ackNo,win;
        uint32_t flag;
        char buf[495] = "";
        int sockfd = fd;
        int len;

        seqNo = 1;
        ackNo = clitcb->ack; 
        win = clitcb->sendWindow;
        flag = PROBE_ACK;
        len = DATAGRAM_LEN;
        

        writePacket(&packet,seqNo,ackNo,win,flag,buf,len);
        sendPacket(sockfd,&packet,len,"PROBE ACK");

}
//Acknowledging FIN
static void sendFinAck(tcb *clitcb,int fd){
        pack_struct packet;
        uint32_t seqNo,ackNo,win;
        uint32_t flag;
        char buf[495] = "";
        int sockfd = fd;
        int len;

        seqNo = 1;
        ackNo = clitcb->ack; 
        win = clitcb->sendWindow;
        flag = FIN_ACK;
        len = DATAGRAM_LEN;
	printf("Sending FIN %d,",flag);     
   

        writePacket(&packet,seqNo,ackNo,win,flag,NULL,len);
        sendPacket(sockfd,&packet,len,"FIN ACK");
}

	
int receiveFile(int *sckfd,int cliwindow,tcb *clitcb){
        pack_struct packet;
        uint32_t seqNo,ackNo,win;
        uint32_t flag;
        char buf[495];
        int sockfd = *sckfd;
        int len;

        while (1){
                len = recvPacket(sockfd,&packet,DATAGRAM_LEN,"DATA PACKET");
                readPacket(&packet,len,&seqNo,&ackNo,&win,&flag,buf);

                printf("Received data : %s \n",buf);
                if (flag == FIN){
                        break;
                }
        }
        return 1;
}

//Updating parameters on data packet arrival

static int update_tcb_parameters(tcb *clitcb,pack_struct *packet,int size) {

        //pack_struct packet = packet1;
        unsigned int seqNum,ackNum,win;
        char recvbuf[PAYLOAD];
        int len = DATAGRAM_LEN;
	char buf[495];
	int flag;

        //len = readPacket(&packet,seqNum,ackNum,flags,win);
	len = readPacket(packet,len,&seqNum,&ackNum,&win,&flag,buf);

        if (flag == FIN) {
                printf("FIN received");
                //return FIN;
        } else if (flag == PROBE) {
                printf("Probe received");
                return PROBE;
        }
	pckQueue *head = clitcb->queueHead;
	int nodeIndex = seqNum%(clitcb->rcvWindow);
	pckQueue *node = &head[nodeIndex];

	if (node->present == 1){
		printf("\n Duplicate Packet received \n");
	}
	//print_packet(*packet); 

        if (clitcb->ack > seqNum) {
                printf(" \n Out of Order packet(older) received ack %d, seq rcv %d \n",clitcb->ack,seqNum);
        } else if (clitcb->ack + clitcb->rcvWindow < seqNum){
		printf("out of window packet received");
        } else {
		writePacket(&node->packet,seqNum,ackNum,win,flag,buf,size);
		node->present = 1;

		if (clitcb->ack == seqNum) {
			do {
				clitcb->ack++;
				clitcb->sendWindow--;
				clitcb->sendWindow = max(clitcb->sendWindow,0);
				nodeIndex = (clitcb->ack)%(clitcb->rcvWindow);
				node = &head[nodeIndex];
				seqNum = node->packet.seqNum;
                       } while((node->present == 1) && (clitcb->ack == seqNum));
		} else {
	               flag = DUP_ACKS;
		}
		//printf("New packet added to receive queue\n");
	}
        return flag;
}

//Data thread responsible for receiving data packets and taking suitable actions
static void *dataManager(void *args){

        pack_struct packet;
        uint32_t seqNo,ackNo,win,flag;
        char buf[PAYLOAD];
        int len;

        struct thread_arguments *argv = (struct thread_arguments *)args;
        int sockfd = *(argv->fd);
        tcb *clitcb = argv->clitcb;
	pckQueue *receiveQ = clitcb->queueHead;

        for(;;){
                len = recvPacket(sockfd,&packet,DATAGRAM_LEN,"DATA PACKET");

		//print_packet(packet);

                pthread_mutex_lock(&tcbMutex);
	
                readPacket(&packet,len,&seqNo,&ackNo,&win,&flag,buf);
		if (isPacketLost() == 1){
			print_packet(packet);
			print_tcb(clitcb,1);
			pthread_mutex_unlock(&tcbMutex);
			continue;

		}

                //printf("Received data : %s \n",buf);

                flag  = update_tcb_parameters(clitcb,&packet,len);
		 print_packet(packet);
                 print_tcb(clitcb,1);


                pthread_mutex_unlock(&tcbMutex);
	
		int flag = packet.flag;
                if (flag == FIN) {
			if (isPacketLost() == 0){
                        	sendFinAck(clitcb,sockfd);
				return;
                        	break;
			}
                } else if (flag == PROBE) {
                        	send_probe_acks(clitcb,sockfd);
                } else if (flag == DUP_ACKS){
			fast_retransmit_acks(clitcb,sockfd);
		} else {
			if (isPacketLost() == 0)
                        	send_acks(clitcb,sockfd);
                }
        }
}

//Validity thread, it prints data on screen and keep track of data printed, sleeps often
static void *validityManager(void *arg){
        pack_struct packet;
        unsigned int seqNum,ackNum,win;
        char recvbuf[PAYLOAD];
        int len,flag;

        struct thread_arguments *argv = (struct thread_arguments *)arg;
        int sockfd = *(argv->fd);
        tcb *clitcb = argv->clitcb;
	int count = 0;
	//double sleeptime;
        pckQueue *head = clitcb->queueHead;
        int nodeIndex;
        pckQueue *node;
	srand(seed);
        for(;;){
                sleeptime = (-1* exp_distributor * log(drand48()));
                //printf("sleep value generated:%lf",sleeptime);
		sleeptime = floor(sleeptime);
                usleep(sleeptime*1000);

		pthread_mutex_lock(&tcbMutex);


		if (clitcb->seq != clitcb->ack) {

                	while (clitcb->seq != (clitcb->ack)) {

        			nodeIndex = (clitcb->seq)%(clitcb->rcvWindow);
        			node = &head[nodeIndex];
				readPacket(&node->packet,len,&seqNum,&ackNum,&win,&flag,recvbuf);
				//printf(" \n Reading Packet from queue \n");	
				//print_packet(node->packet);
                		printf("%s",recvbuf);
			        node->present = 0;	
                		clitcb->sendWindow++;
                		clitcb->seq++;
				if (flag == FIN) {
                                        pthread_mutex_unlock(&tcbMutex);
                                        return;
                                }
				memset(node,0,sizeof(pckQueue));
         		}
		}
                pthread_mutex_unlock(&tcbMutex);
        }
}
/* Main function to initialize file
* Creates two threads 1. One for receiving packets and other for printing data
* Synchronized thread via locks
*/
int receive_file(int *sockfd,int winsize,tcb *clitcb) {

        pthread_t dataThread;
        pthread_t validityThread;
        struct thread_arguments *argv = (struct thread_arguments *)malloc(sizeof(struct thread_arguments));
        argv->clitcb = clitcb;
        argv->fd = sockfd;


        pthread_create(&dataThread,NULL,&dataManager,(void *)argv);
        pthread_create(&validityThread,NULL,&validityManager,(void *)argv);

        pthread_join(dataThread,NULL);
        pthread_join(validityThread,NULL);

        printf("File transfer completed \n");
        return 1;

} 


