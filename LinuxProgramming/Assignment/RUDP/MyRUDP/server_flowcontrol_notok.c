#include "server.h"
#include "util.h"
#include <sys/stat.h>
#include <fcntl.h>

/* Function prototype of server program 
   For more the details, you can see in client program because of the sames
*/
static void signal_hello_function(int sigNum);
static int read_server_params ();
static int init_server ();//struct sockaddr_in server_addr);
static void ser_hand_shake(int sockfd, struct sockaddr *addr, int addLen);//, pckStruct_t pck_sent, pckStruct_t pck_rcvd);
static void signal_hello_function(int sigNum);
static void ser_rudp_handler (int sockfd, struct sockaddr *addr, int addLen);

static void signal_hello_function(int sigNum){
    siglongjmp (hello_signal, 1);
}

static int read_server_params () {
	FILE *fp;
	int lineNo = 1;
	size_t len;
	char buf[MAXSIZE];
	int i = 0;

	fp = fopen (SERVER_INPUT, "rt");
	if (!fp) {
		return -1;
	}

	while (fgets(buf, MAXSIZE, fp)) {
		if (lineNo == 1) 
			serverPort = atoi (buf);
		else if (lineNo == 2)
			swnd = atoi (buf);
		else
			strncpy (req_file_name, buf, MAX_FILENAME_LEN);
		lineNo ++;
		//bzero (buf, MAXSIZE);
	}
	printf("SERVER PARAMS:\n");
	printf("Server Port: %d\n", serverPort);
	printf("swnd: %d\n", swnd);
	printf("Requested file: %s\n", req_file_name);
}

static int init_server () {//struct sockaddr_in server_addr) {
	int sockfd, buf_size, optval = 1;
	struct ifi_info *ifi;
	struct sockaddr_in server_addr;
	buf_size = BUF_SIZE;

	//ifi = Get_ifi_info_plus(AF_INET, 1);

	/* Create socket */
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("socket failed");
		exit(1);
	}
	printf("\n\n---------------\nServer successfully create socket\n");

	setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof (optval));
	
	/* Set buffer size */
	if (setsockopt (sockfd, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof (buf_size)) < 0)
		perror ("Server: Failed to set recv buf\n");

	if (setsockopt (sockfd, SOL_SOCKET, SO_SNDBUF, &buf_size, sizeof (buf_size)) < 0)
		perror ("Server: Failed to set send buf\n");

	/* Assign port and bind */
	bzero( &server_addr,sizeof(server_addr));
	server_addr.sin_family  		= AF_INET;
	server_addr.sin_port 		    = htons(serverPort);
	//server_addr = *(struct sockaddr_in *)ifi->ifi_addr;

	if (bind (sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
		perror("bind failed");
		exit(1);
	}	
	printf("Server bind successfully\nWaiting for incomming connection...\n");
	return sockfd;
	
}

static void ser_hand_shake(int sockfd, struct sockaddr *addr, int addLen) { //, pckStruct_t pck_sent, pckStruct_t pck_rcvd) {

	uint32_t seqNo, ackNo, win, flag;
    int len;
    char buf[496];
    pckStruct_t pck_sent;
	pckStruct_t pck_rcvd;

	struct itimerval *timer;
    sigset_t timer_signal_set;
    char *pck_type;

	/* Running timers to keep track of lost packets */
    signal(SIGALRM,signal_hello_function); // use signal_hello_function function to handle SIGALRM
    sigemptyset(&timer_signal_set);
    sigaddset(&timer_signal_set,SIGALRM);

    timer = (struct itimerval *) malloc (sizeof (struct itimerval));
    int time_start = TIMER_FIN;

receive_first_hello:
	//Step 1: receive first syn signal (SYN) from client
	if (recvfrom(sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
		perror("recv failed");
		exit(1);
	}
	read_packet (pck_rcvd, &seqNo, &ackNo, &win, &flag, buf);
	printf("\n\n\n---------------------------\n+++received: %s", buf);

send_second_hello:
	//Step 2: send SYN_ACK
	if (pck_rcvd.type == SYN) {
		pck_type = "SYN_ACK";
		setjmp (normal_buf);
		puts("\n\n\n---------------------------\n---send SYN_ACK");
		init_timer (timer, time_start);
		write_packet(&pck_sent, 0, 0, swnd, SYN_ACK, "SECOND HANDSHAKE: SYN_ACK");
		
		/* Check if SYN_ACK is lost? */
		if (is_packet_loss(time_start, pck_type)) {
			goto time_out;
		}
		if (sendto(sockfd, &pck_sent, sizeof(pck_sent), 0, addr, addLen) < 0) {
			perror("send failed");
			exit(1);
		}
	}
time_out: /* Read note in cli_hand_shake function, inside client program */
	time_out_handler (PEER_HOST, pck_type);

receive_third_hello:	
	//Step 3: receive SYN_ACK_ACK
	sigprocmask(SIG_UNBLOCK,&timer_signal_set,NULL);
        
	//pck_rcvdet(sockfd,packet,DATAGRAM_LEN,"Third Hand-Shake");
	if (recvfrom(sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
		perror("recv failed");
		exit(1);
	}	

	read_packet (pck_rcvd, &seqNo, &ackNo, &win, &flag, buf);
	sigprocmask(SIG_BLOCK, &timer_signal_set, NULL);

	if (pck_rcvd.type == SYN_ACK_ACK) {
		puts("\n\n\n---------------------------\n+++received SYN_ACK_ACK. \n---Finish handshaking!");
	}
	else
		goto receive_third_hello;
	stop_timer (timer);
    sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);
}

static void ser_rudp_handler (int sockfd, struct sockaddr *addr, int addLen) {
	
	FILE *rq_file_fp; /* pointer of requested file */
	uint32_t seqNo, ackNo, win, flag, base;
    int len;
    char buf[PAYLOAD];
    char *pck_type;
	int temp;
    pckStruct_t pck_sent;
	pckStruct_t pck_rcvd;
	seqNo = 1, ackNo = 1; 
	base = 1;

	struct itimerval *timer;
    sigset_t timer_signal_set;
    

    rq_file_fp = fopen (req_file_name, "rt");
    if (!rq_file_fp) {
		exit (1);
	}

    puts("---prepare send data to the client...");	
    // initiate rwnd
    pck_rcvd_rwnd = WINSIZE;
    swnd = WINSIZE;  
    while (1) {

		/* Send data packet */
		send_data_packet:

			pck_type = "DATA";
			printf("---current base: %d\n", base);		
			//while(seqNo < base + WINSIZE){
			while(seqNo < base + pck_rcvd_rwnd){    /* WARNING: next, we have to consider swnd */
				//setjmp (normal_buf);
				//init_timer (timer, time_start);
				memset(buf, '\0', PAYLOAD);
				fgets(buf, PAYLOAD, rq_file_fp);
				write_packet(&pck_sent, seqNo ++, ackNo, swnd, DATA, buf);
				printf ("\n\n---------------------------\n---send packet: "); 
				print_packet (pck_sent);

				/* Check if packet is lost? */
				/*if (is_packet_loss(time_start, pck_type)) {
					goto time_out;
				}*/
				if (sendto(sockfd, &pck_sent, sizeof(pck_sent), 0, addr, addLen) < 0) {
					perror("send failed");
					exit(1);
				}

				//time_out: /* Read note in cli_hand_shake function, inside client program */
					//time_out_handler (PEER_HOST, pck_type);
			}

		/* Receive ack */
		receive_ack:
			if (recvfrom(sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
				perror("recv failed");
				exit(1);
			}
			//printf("check temp %d \n", temp);

			//update base if received appropriate ACK
			if(pck_rcvd.ackNum == base){
				base++;
			}
			pck_rcvd_rwnd = pck_rcvd.winSize;
			printf ("\n\n---------------------------\n+++receive packet: "); 
			print_packet (pck_rcvd);	
			
			if(*buf == '*'){
				break;
			}
	}
	fclose(rq_file_fp);
	puts ("Finish send data to client");
	close(sockfd);
	
}

int main(int argc, char const *argv[])
{
	int sockfd, addr_len;
	//struct in_addr *serv_addr;
	//struct in_addr client_addr;
	struct sockaddr_in server_addr, client_addr;
	//char addr[INET_ADDRSTRLEN];
	

	pckStruct_t pck_sent;
	pckStruct_t pck_rcvd;

	//Get the parmeters from client.in
	read_server_params ();

	sockfd = init_server (); 

	addr_len = sizeof (server_addr);
	ser_hand_shake (sockfd, (struct sockaddr *) &client_addr, addr_len);
	ser_rudp_handler (sockfd, (struct sockaddr *) &client_addr, addr_len);
	
	return 0;
}