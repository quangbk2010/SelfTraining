#include "util.h"
#include "unprtt.h"
#include <signal.h>
#include <setjmp.h>
#include "server.h"
tcb *sertcb;
pckQueue *head;
pckQueue *node;
int nodeIndex;
// Declaring signals for transmission and termination
sigjmp_buf transmit_file_signal;
sigjmp_buf termination_signal;

static void signal_transmit_file(int sigNum){
        siglongjmp(transmit_file_signal,1);
}

static void signal_conn_close(int sigNum){
        siglongjmp(termination_signal,1);
}
// Initializing server
int initilaize_server_tcb(tcb *ser_tcb, uint32_t cliwin,uint32_t servwin) {

        ser_tcb->nextSendSeqNum = 1;
	ser_tcb->oldestSeqNum   = 1;
	ser_tcb->nextNewSeqNum  = 1;

        ser_tcb->ack = 1;

        ser_tcb->rcvWindow = cliwin;
        ser_tcb->sendWindow = servwin;
	ser_tcb->ssThresh   = servwin;
	ser_tcb->congWindow = 1; 

        // sertcb->seq = 0;

        ser_tcb->queueHead = (struct pckQueue *) calloc(servwin,sizeof(struct pckQueue));

        return 1;
}
// Sending Probes
void send_probe(tcb *sertcb,int fd) {
        pack_struct packet;
        uint32_t seqNo,ackNo,win;
        uint32_t flag;
        char buf[495];
        int len_no;

        seqNo = sertcb->nextSendSeqNum;
        ackNo = sertcb->ack;
        win = sertcb->sendWindow;
        flag = PROBE;
        len_no = DATAGRAM_LEN;


        writePacket(&packet,seqNo,ackNo,win,flag,buf,len_no);
        sendPacket(fd,&packet,len_no,"PROBE ACK");

}

//Update TCB parameters
static pckQueue *update_tcb_parameters(tcb *sertcb,pack_struct *packet,int size) {

        unsigned int seqNum,ackNum,win;
        char recvbuf[PAYLOAD];
        int len = DATAGRAM_LEN;
        char buf[495];
        int flag;


        pckQueue *head = sertcb->queueHead;
        int nodeIndex = seqNum%(sertcb->rcvWindow);
        pckQueue *node = &head[nodeIndex];
	seqNum = packet->seqNum;
	ackNum = packet->ackNum;
	win = packet->winSize;
	flag = DATA;
	strcpy(buf,packet->payload);
	size = DATAGRAM_LEN;
	
        writePacket(&node->packet,seqNum,ackNum,win,flag,buf,size);
        node->present = 1;
	node->noOfRetransmits = 0;
	sertcb->nextSendSeqNum++;

	if (sertcb->nextNewSeqNum == packet->seqNum){
		sertcb->nextNewSeqNum++;
	}
	return node;
}

//Terminating Connection
void sendFin(tcb *clitcb,int fd){
        pack_struct packet;
        uint32_t seqNo,ackNo,win;
        uint32_t flag;
        char buf[495];
        int sockfd = fd;
        int len;
	struct itimerval *timer;
	int fin_retransmits = 0;
        seqNo = clitcb->nextSendSeqNum;
        ackNo = clitcb->ack;
        win = clitcb->sendWindow;
        flag = FIN;
        len = DATAGRAM_LEN;
	sigset_t timer_signal_set;
	Signal(SIGALRM,signal_conn_close);
       sigemptyset(&timer_signal_set);
        sigaddset(&timer_signal_set,SIGALRM);


	timer = (struct itimerval *)Malloc(sizeof(struct itimerval));
        long int time_start = TIMER_FIN;
        timer->it_interval.tv_sec = 0;
        timer->it_interval.tv_usec = 0;
        timer->it_value.tv_sec = time_start/1000;
        timer->it_value.tv_usec = (time_start % 1000)*1000;


	//long int time_start = rtt_start(&rttInfo);

Retransmit_FIN:
        writePacket(&packet,seqNo,ackNo,win,flag,NULL,len);
        sendPacket(sockfd,&packet,len,"FIN");
	printf("Sending FIN");
        initialize_timer(timer,time_start);
        //setitimer(ITIMER_REAL,timer,0);

	if (sigsetjmp(termination_signal,1) != 0){
			fin_retransmits++;
                        printf("FIN Timeout occurred");
			if (fin_retransmits >= RETRANS_MAX){
				printf("\n Client Down, not sending ACK");
				return;
			}
		goto Retransmit_FIN;

	}
	int flag_new;
        while(1){
		sigprocmask(SIG_UNBLOCK,&timer_signal_set,NULL);
	       len = recvPacket(fd,&packet,DATAGRAM_LEN,"FIN ACK PACKET");
               len = readPacket(&packet,len,&seqNo,&ackNo,&win,&flag_new,buf);
		sigprocmask(SIG_BLOCK,&timer_signal_set,NULL);

		//printf("\n received flag is %d, " ,flag);
		if (flag == FIN_ACK || flag == FIN)
			break;
	}	

	time_start = 0;
        timer->it_interval.tv_sec = 0;
        timer->it_interval.tv_usec = 0;
        timer->it_value.tv_sec = time_start/1000;
        timer->it_value.tv_usec = (time_start % 1000)*1000;

	printf("\n FIN received \n");
	initialize_timer(timer,0);
	sigprocmask(SIG_UNBLOCK,&timer_signal_set,NULL);
	//setitimer(ITIMER_REAL,timer,0);
}

//Code to handle congestion window increments
static void increse_congestion_window(tcb *sertcb) {

        if (sertcb->congWindow >= sertcb->ssThresh){
                        if (sertcb->oldestSeqNum == sertcb->nextNewSeqNum){
                                sertcb->congWindow = min(sertcb->congWindow+1,sertcb->rcvWindow);
                        }
        } else {
                sertcb->congWindow = min(sertcb->congWindow+1,sertcb->rcvWindow);


        }
        sertcb->congWindow = min(sertcb->congWindow,sertcb->sendWindow);

}
//Reseting nodes
static void reset_nodes(tcb *sertcb){
        int i;
        for (i = 0;i < sertcb->sendWindow;i++){
                sertcb->queueHead[i].noOfRetransmits = 0;
                memset(&sertcb->queueHead[i],0,sizeof(pckQueue));
        }
}


//Handling retransmissions
pckQueue *handle_retransmissions(tcb *sertcb,struct rtt_info *rttInfo) {

		       pckQueue *head;
        		head = sertcb->queueHead;
        		int nodeIndex;
        		pckQueue *node;

                         nodeIndex = (sertcb->nextSendSeqNum)%(sertcb->sendWindow);
                         node = &head[nodeIndex];
                         printf(" \n retransmission node index %d, node payoad %s\n",nodeIndex,node->packet.payload);
                         node->noOfRetransmits++;
                         node->timestamp = rtt_ts(rttInfo);
                         sertcb->nextSendSeqNum++;
			return node;
}

//main function to handle server data generation and receiving acks
/* This is long and hard part of code.
* It is divided into three parts 1. Send Data 2. Handle retransmission 3. Receive Acks
* These three parts run in loop untill all data is sent out
*/
int transmit_data(int *sock_fd,int filefd,struct rtt_info rttInfo,tcb *ser_tcb) {

        int i;
	//tcb *sertcb;
	sertcb = ser_tcb;
        //pckQueue *head;

        head = sertcb->queueHead;
        //int nodeIndex;
        //pckQueue *node;

        int flag;
        char buf[495];
	int sockfd = *sock_fd;
	int n;
	struct itimerval *timer;
        timer = (struct itimerval *)Malloc(sizeof(struct itimerval));
	sigset_t timer_signal_set;
	int finflag = 0;
//Declare signal alarm for retranmission
	Signal(SIGALRM,signal_transmit_file);
	sigemptyset(&timer_signal_set);
	sigaddset(&timer_signal_set,SIGALRM);
	
	
        pckQueue *pack;
	rttInfo.rtt_rto = 3000;
	//long int time_start = rtt_start(&rttInfo);
	long int time_start = 2000;
 	timer->it_interval.tv_sec = 0;
	timer->it_interval.tv_usec = 0;
	timer->it_value.tv_sec = time_start/1000;
	timer->it_value.tv_usec = (time_start % 1000)*1000;
        pack_struct packet;
	
	uint32_t packsent;

	uint32_t seqNum;
	uint32_t len;
	uint32_t winSize;
	uint32_t ackNum;
	uint32_t duplicate_acks;
	int completed = 0;
	

 

//Start the process
       while (completed == 0) {
Retransmission:
                if (sertcb->rcvWindow <= 0) {
			//handling probes
                        usleep(TIMER_PROBE*1000);
                        send_probe(sertcb,sockfd);
                } else {
			// packet transfer mechanism started
		        if (sertcb->nextSendSeqNum < sertcb->nextNewSeqNum){

				sertcb->nextSendSeqNum = sertcb->oldestSeqNum;

			}
			int window_packet = min(sertcb->congWindow,sertcb->rcvWindow);
 
                        for (i = 0;i < window_packet;i++){

					if (sertcb->nextSendSeqNum < sertcb->nextNewSeqNum) {
					
						//Retransmitting unacked packets	
				                //node = handle_retransmissions(sertcb,&rttInfo);
						nodeIndex = (sertcb->nextSendSeqNum)%(sertcb->sendWindow);
						node = &head[nodeIndex];
						//printf(" \n retransmission \n");
						printf(" \n RETRANSMISSION " );

						node->noOfRetransmits++;
						node->timestamp = rtt_ts(&rttInfo);
						sertcb->nextSendSeqNum++; 
					} else {

                                        	char filedata[495];
						memset(filedata,'\0',495);
                                        	n = Read(filefd,filedata,490);
						//check if read has reached end of file
                                        	if (n <= 0 ){
							completed = 1;
					        	//sendFin(sertcb,sockfd);
							finflag = 1;	
                                                	break;
                                        	}
                                        	flag = DATA;
                                        	writePacket(&packet,sertcb->nextSendSeqNum,sertcb->ack,sertcb->sendWindow,DATA,filedata,DATAGRAM_LEN);
						//node = update_tcb_parameters(sertcb,&packet,DATAGRAM_LEN);
						 unsigned int seqNum,ackNum,win;
					        char recvbuf[PAYLOAD];
        					int len = DATAGRAM_LEN;
        					char buf[495];
        					int flag;


        					//pckQueue *head = sertcb->queueHead;
        					nodeIndex = (sertcb->nextSendSeqNum)%(sertcb->sendWindow);
        					node = &head[nodeIndex];
        					seqNum = packet.seqNum;
        					ackNum = packet.ackNum;
        					win = packet.winSize;
        					flag = DATA;
        					strcpy(buf,packet.payload);
        					int size = DATAGRAM_LEN;

        					writePacket(&node->packet,seqNum,ackNum,win,flag,buf,size);
        					node->present = 1;
        					node->noOfRetransmits = 0;
        					sertcb->nextSendSeqNum++;

        					if (sertcb->nextNewSeqNum == packet.seqNum){
                				sertcb->nextNewSeqNum++;
        					}

				   		//print_packet(node->packet);
					}

                                    	sendPacket(sockfd,&node->packet,len,"DATA PACKET");
				        print_packet(node->packet);
 				        print_tcb(sertcb,2);
					// Put timestamp on nodes
                                    	node->timestamp = rtt_ts(&rttInfo);
                                    	seqNum++;
					if (n < 490){
						//sendFin(sertcb,sockfd);
						completed = 1;
						finflag = 1;
						break;
					}

                       }

		}

		Signal(SIGALRM,signal_transmit_file);
		initialize_timer(timer,time_start);

		// Retransmission handler
		if (sigsetjmp(transmit_file_signal,1) != 0){
			printf("Timeout occurred");

			if (sertcb->rcvWindow > 0 ){
                                nodeIndex = sertcb->oldestSeqNum%sertcb->sendWindow;
                                node = &sertcb->queueHead[nodeIndex];
                                int noofretransmits = node->noOfRetransmits;
                                 if (rtt_timeout(&rttInfo,noofretransmits) == - 1){
                                        printf(" \n RETRANSMISSION TIMER FIRED, CLIENT DOWN \n");
                                        break;
                                }
                                completed = 0;

                                //congestion control mechanism
				printf("\n ENTERING SLOWSTART AFTER RETRANSMISSION");
                                sertcb->ssThresh = max(sertcb->congWindow/2,1);
                                sertcb->congWindow = 1;
                                sertcb->nextSendSeqNum = sertcb->oldestSeqNum;
				print_tcb(sertcb,2);
                        }
                        goto Retransmission;
		}
		//Retransmission handler completed

                //Receive ACKS mechanism

		//resetting duplicate acks before starting ack mechanism
		duplicate_acks = 0;
	             while(1) {
                        char buf[495];
			memset(&packet,0,sizeof(pack_struct));
			uint32_t seqNum;
			uint32_t ackNum;
			uint32_t winSize;
			uint32_t flag;
			int len;

			sigprocmask(SIG_UNBLOCK,&timer_signal_set,NULL);
			 len = recvPacket(sockfd,&packet,DATAGRAM_LEN,"ACK PACKET");
                         len = readPacket(&packet,len,&seqNum,&ackNum,&winSize,&flag,buf);
			sigprocmask(SIG_BLOCK,&timer_signal_set,NULL);
	   	        print_packet(packet);
                        //print_tcb(sertcb,2);

			//Update Window based on client window - Flow  Control	
                        sertcb->rcvWindow = winSize;

			if (flag == PROBE_ACK) {
				printf(" \n PROBE ACK ");
				print_tcb(sertcb,2);
			} else {
				 
				if (sertcb->oldestSeqNum == ackNum) {
					//Fast Recovery, handle duplicate Acks
					duplicate_acks++;
					if (duplicate_acks == 3) {
						completed = 0;
					
						sertcb->nextSendSeqNum = sertcb->oldestSeqNum;	
						sertcb->ssThresh = max(sertcb->congWindow/2,1);
						sertcb->congWindow = max(sertcb->ssThresh,1);
						printf(" \n ENTERING FAST RECOVERY \n");
						print_tcb(sertcb,2);
						break;
					} else {
						printf("\n DUPLICATE ACK RECEIVED");
					}
				} else {
				         //handle new acks	
					//handling cumulative acks:
                                        while(sertcb->oldestSeqNum < ackNum ){
                                                nodeIndex = sertcb->oldestSeqNum%(sertcb->sendWindow);
                                                node = &head[nodeIndex];
						if ( node != NULL){
							rtt_stop(&rttInfo,node->timestamp);
                                                	node->present = 0;
						}
                                                sertcb->oldestSeqNum++;
						//increase congestion window on new acks
                                                increse_congestion_window(sertcb);
                                        }
				}
				print_tcb(sertcb,2);


                                        if (sertcb->nextSendSeqNum < sertcb->oldestSeqNum)
                                                sertcb->nextSendSeqNum = sertcb->oldestSeqNum;
//Making sure, we receive all acks before moving to sending data again in window 
                                        if (sertcb->oldestSeqNum != sertcb->nextNewSeqNum){
                                                continue;
                                        } else {
						if (completed && finflag == 1 )
							sendFin(sertcb,sockfd);
					}
						
                                        break;
                        }
		}

	//Set the timer value to 0;
	initialize_timer(timer,0);
	//Unblock the timer signal
        sigprocmask(SIG_UNBLOCK,&timer_signal_set,NULL);

	}
	return 1;
}
