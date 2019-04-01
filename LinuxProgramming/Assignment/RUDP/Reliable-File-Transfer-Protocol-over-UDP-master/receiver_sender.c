#include "util.h"
#include <time.h>
#include "unprtt.h"

void print_tcb(tcb *stcb,int type) {
	if (type == 1) {
		printf("\n Current Ack no %d , Current Receive window %d ",stcb->ack,stcb->rcvWindow);

	} else {
	uint32_t win = stcb->rcvWindow;
	if (win < 0)
		win = win*(-1);
		printf("\n  Seqno %d,Last unacked %d,RcvWindow %d, CongWindow %d,ssThresh %d \n",stcb->nextNewSeqNum,stcb->oldestSeqNum,win,stcb->congWindow,stcb->ssThresh);
	}
}

void print_packet(pack_struct packet){
	printf(" \n SeqNum is %d , AckNo is %d , flag is %d,window is %d ",packet.seqNum,packet.ackNum,packet.flag,packet.winSize);

}
int writePacket(pack_struct *packet,uint32_t seqNo,uint32_t ackNo,uint32_t win,uint32_t flag,char *msg,int len){
	//pack_struct *pack;
	//pack = packet;
	packet->seqNum = seqNo;
	packet->ackNum = ackNo;
	packet->winSize = win;
	packet->flag = flag;
	memset(packet->payload,'\0',PAYLOAD);
	if (msg != NULL){
		strcpy(packet->payload, msg);
	}

	return 1;
	
}
int sendPacket(int fd,pack_struct *packet,int size,char *msg) {

	printf("\n SENDING PACKET %s ",msg);
	//print_packet(*packet);	
	write(fd,packet,DATAGRAM_LEN);
	return 1;

}

int recvPacket(int fd,pack_struct *packet,int size,char *msg) {
        int len = -1;
	while (len < 0) {
	memset((void *)packet,'\0',DATAGRAM_LEN);	
	 len = read(fd,(void *)packet,size);
	}
	printf("\n RECEIVED PACKET %s ",msg);
	return len;

}
int readPacket(pack_struct *packet,uint32_t len,uint32_t *seqNo,uint32_t *ackno,uint32_t *win,uint32_t *flag,char *buf) {
      //printf("\n inside read packet \n");

        *seqNo = packet->seqNum;
	*ackno = packet->ackNum;
	*win  = packet->winSize;
	*flag = packet->flag;
	strcpy(buf,packet->payload);
	//buf = packet->payload;
	return 1;
	
}

int initialize_timer(struct itimerval *itimer,long int time_start){

        itimer->it_interval.tv_sec = 0;
        itimer->it_interval.tv_usec = 0;
        itimer->it_value.tv_sec = time_start/1000;
        itimer->it_value.tv_usec = (time_start % 1000)*1000;
	setitimer(ITIMER_REAL,itimer,0);

}
