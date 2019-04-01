#include "util.h"

void print_packet (pckStruct_t packet) {
	if (packet.type == DATA || packet.type == PROBE)
		printf("SeqNum: %d, Flag: %d, Rwnd: %d, payload: %s\n", packet.seqNum, packet.type, packet.winSize, packet.payload);
	else if (packet.type == ACK || packet.type == PROBE_ACK)
		printf("ackNum: %d, Flag: %d, Rwnd: %d\n", packet.ackNum, packet.type, packet.winSize);
	else
		printf("Flag: %d, Rwnd: %d\n", packet.type, packet.winSize);
}
int write_packet (pckStruct_t *packet, uint32_t seqNum, uint32_t ackNum, uint32_t winSize, uint32_t type, char *data) {
	packet -> seqNum = seqNum;
	packet -> ackNum = ackNum;
	packet -> winSize = winSize;
	packet -> type = type;
	memset (packet -> payload, '\0', PAYLOAD);
	if (data != NULL)
		strcpy (packet -> payload, data);
	return PAYLOAD;
}
int read_packet (pckStruct_t packet, uint32_t *seqNum, uint32_t *ackNum, uint32_t *winSize, uint32_t *type, char *data) {
	*seqNum = packet.seqNum;
	*ackNum = packet.ackNum;
	*winSize = packet.winSize;
	*type = packet.type;
	strcpy (data, packet.payload);
	return PAYLOAD;
}

/*int send_packet (int sockfd, pckStruct_t *packet, char *msg) {
	printf("\nSENDING PACKET: %s\n", msg);
	write (sockfd, (void *) packet, PCK_SIZE);
	return PCK_SIZE;
}
int recv_packet (int sockfd, pckStruct_t *packet, char *msg) {
	printf("\nRECEIVING PACKET: %s\n", msg);
	memset ((void *) packet, '\0', PCK_SIZE);
	return (read (sockfd, (void *) packet, PCK_SIZE));
}*/

void init_timer (struct itimerval *iTimer, int timeStart) {
	iTimer -> it_interval.tv_sec = 0;
	iTimer -> it_interval.tv_usec = 0;
	iTimer -> it_value.tv_sec = timeStart/1000;
	iTimer -> it_value.tv_usec = (timeStart % 1000) * 1000;
	setitimer (ITIMER_REAL, iTimer, 0);
}

void stop_timer (struct itimerval *iTimer) {
	init_timer (iTimer, 0);
}

int is_packet_loss(int time_out, char *msg) {//int sockfd, pckStruct_t *packet, int type, struct sockaddr *address, int addLen){
	int randnum = rand()%100 + 1;
	
    if(LOSSRATE < randnum) {
        printf("---package sent successfully\n");
        return 0;
    }
    else {
        printf("---package loss: %s, time_out (ms) = %d\n", msg, time_out);
        return 1;
    }
}

void time_out_handler (int *retransmitNo, char *peer_host, char *pck) {
	static char *name_peer_host;
	static char *type_pck;
	static int *reNo;
	reNo = retransmitNo;
	type_pck = pck;
	name_peer_host = peer_host;

	if ( sigsetjmp(hello_bye_signal,1) != 0) {
		puts ("\n\n============================time_out_handler============================");
        (*reNo)++;
        printf("===Timeout occurred with: %s\n", type_pck);
        if ((*reNo) >= RETRANS_MAX){
            printf("\n=== %s Down, not sending ACK\n", name_peer_host);
            exit(1);
        }
       longjmp (normal_buf, 1);
    }
}


void current_time(struct timeval *t_current)
{
	gettimeofday(t_current, NULL);
}


int get_durration(struct timeval *t_begin, struct timeval *t_end)
{
	return (int)(((unsigned long long)(t_end->tv_sec) * 1000 +
	       (unsigned long long)(t_end->tv_usec) / 1000) -
               ((unsigned long long)(t_begin->tv_sec) * 1000 +
		(unsigned long long)(t_begin->tv_usec) / 1000));
}

int sendUnreliably(int socket, pckStruct_t *packet, struct sockaddr *address, int addLen) {
	int randnum = rand()%100 + 1;
	
    if(LOSSRATE < randnum){
        if(sendto(socket, packet, sizeof(pckStruct_t), 0, address, addLen) < 0){
            return -1;
        }
        printf("---package sent\n");
        return 1;
    }else{
        printf("---package loss\n");
        return 0;
    }
}

void make_rtt (int ms) {
	usleep (ms * 1000);
}
