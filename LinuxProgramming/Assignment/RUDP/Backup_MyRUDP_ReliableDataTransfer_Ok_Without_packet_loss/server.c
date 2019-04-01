#include "server.h"
#include "util.h"
#include <sys/stat.h>
#include <fcntl.h>

/* Function prototype of server program 
   For more the details, you can see in client program because of the sames
*/
static void signal_hello_bye_function(int sigNum);
static int updateDataArray(int base, int seqNo, FILE *in, int type, char dataArray[SENDBUFFER][PAYLOAD], int rwnd);
static int read_server_params ();
static int init_server ();//struct sockaddr_in server_addr);
static void ser_hand_shake(int sockfd, struct sockaddr *addr, int addLen);//, pckStruct_t pck_sent, pckStruct_t pck_rcvd);
static void signal_hello_function(int sigNum);
static void ser_rudp_handler (int sockfd, struct sockaddr *addr, int addLen);
static void ser_terminate(int sockfd, struct sockaddr *addr, int addLen);//, pckStruct_t pck_sent, pckStruct_t pck_rcvd);


/* FUNCTION DECLARATION */

static void signal_hello_bye_function(int sigNum){
    siglongjmp (hello_bye_signal, 1);
}
/* dataArray store the packets which are not sent successfully to the client, and packets which are stored in buffer and will be sent in the next rounds */
static int updateDataArray(int base, int seqNo, FILE *in, int type, char dataArray[SENDBUFFER][PAYLOAD], int rwnd) {
	//shift data in TCP based on BASE and SEQNO
	int i;
	int no_unAck = seqNo - base;
	int mark;
	char temp[PAYLOAD];
	static int old_rwnd = WINSIZE;

	//clear dataArray if base = seqNo
	if (no_unAck == 0 && base == 1) {
		//puts ("-------");
		//memset(dataArray, 0, sizeof(dataArray));
		//clear Window from distance
		//for (i = 0; i < SENDBUFFER; i++) {
			//strcpy(dataArray[i], "");
		//}
		//insert data while file is not ended
		for (i = 0; i < SENDBUFFER; i++) { 
			memset(temp, '\0', PAYLOAD);
			fgets(temp, PAYLOAD, in);
			strcpy(dataArray[i], temp);
		}
		//puts ("++++++");
	} 
	else if (type == DATA) {
		mark = old_rwnd - no_unAck;
		if (mark > 1)
			mark = 1;
		// shift data in buffer to the left
		for (i = 0; i < SENDBUFFER - mark; i++) {
			strcpy(dataArray[i], dataArray[i + mark]);
		}
		//clear Window from distance
		for (i = SENDBUFFER - mark; i < SENDBUFFER; i++) {
			strcpy(dataArray[i], "");
		}
		//insert data while file is not ended
		for (i = SENDBUFFER - mark; i < SENDBUFFER; i++) { 
			memset(temp, '\0', PAYLOAD);
			fgets(temp, PAYLOAD, in);
			strcpy(dataArray[i], temp);
		}
	}
	//puts ("2....");
	//for (i = 0; i < SENDBUFFER; i ++) {
	//	printf("a[%d] = %s\t", i, dataArray[i]);
	//}
	//printf("\nold win = %d\n", old_rwnd);
	old_rwnd = rwnd;
	return no_unAck;
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
    signal (SIGALRM, signal_hello_bye_function); // use signal_hello_function function to handle SIGALRM
    sigemptyset (&timer_signal_set);
    sigaddset (&timer_signal_set, SIGALRM);

    timer = (struct itimerval *) malloc (sizeof (struct itimerval));
    int time_start = TIMER_ACK;

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
	sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);
        
	//pck_rcvdet(sockfd,packet,DATAGRAM_LEN,"Third Hand-Shake");
	if (recvfrom (sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
		perror ("recv failed");
		exit (1);
	}	

	read_packet (pck_rcvd, &seqNo, &ackNo, &win, &flag, buf);
	sigprocmask (SIG_BLOCK, &timer_signal_set, NULL);

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
	uint32_t seqNo, ackNo, win, flag, base, no_unAck;
	char dataArray[SENDBUFFER][PAYLOAD], buf[PAYLOAD];
    int len, type, i;
    
    char *pck_type, *temp_payload; // type of packet to be sent out, which is used to printing to the screen 
    pckStruct_t pck_sent;
	pckStruct_t pck_rcvd;
	seqNo = 1, ackNo = 1; 
	base = 1;

	struct itimerval *timer;
    sigset_t timer_signal_set;
    
	/* Running timers to keep track of lost packets */
    //signal (SIGALRM, signal_hello_bye_function); // use signal_hello_function function to handle SIGALRM
    //sigemptyset (&timer_signal_set);
    //sigaddset (&timer_signal_set, SIGALRM);

    timer = (struct itimerval *) malloc (sizeof (struct itimerval));
    int time_start = TIMER_ACK;


    rq_file_fp = fopen (req_file_name, "rt");
    if (!rq_file_fp) {
		exit (1);
	}

    puts("---prepare send data to the client...");	
    // initiate rwnd
    pck_rcvd_rwnd = WINSIZE;
    swnd = WINSIZE;
    type = DATA;
    no_unAck = 0;
	pck_type = "DATA"; // type of packet to be sent out
    while (1) {
    	/* sender have to keep the number of unacknowledged packets less than rwnd */
    	no_unAck = seqNo - base;
    	//printf("#######seqNo: %d, base: %d, pck_rcvd_rwnd: %d, swnd: %d, no_unAck: %d\n", seqNo, base, pck_rcvd_rwnd, swnd, no_unAck);
    	if (pck_rcvd_rwnd > no_unAck) {
    		//puts ("^^^^");
    		swnd = pck_rcvd_rwnd - no_unAck;
    	}
    	else
    		swnd = 0;
    	// if timeout resend current frame
		if (isTimeout) {
			init_timer (timer, time_start);
			isTimeout = 0;
			printf("resend from seqNo: %d \n", base);
			for (i = 0; i < pck_rcvd_rwnd; i++) {
				if (type == DATA)
					temp_payload = dataArray[i];
				else if (type == PROBE)
					temp_payload = "Just for probing";
				write_packet(&pck_sent, base + i, ackNo, swnd, type, temp_payload);
				printf("\n\n---------------------------\n---send packet: ");
				print_packet (pck_sent);
				puts (pck_sent.payload);

				if (sendUnreliably(sockfd, &pck_sent, addr, addLen) < 0) {
					perror("send failed");
					exit(0);
				}

			}

			// if try ==  maxtries, terminate connection
			if (try == RETRANS_MAX) {
				puts("Client is corrupted!");
				puts("Terminate connection!");
				break;
			}
		}
		else {
			try = 0;

			//update array based on BASE and SEQNO
			//puts ("###");
			updateDataArray(base, seqNo, rq_file_fp, type, dataArray, pck_rcvd_rwnd);

			//send the next seqNo
			//puts ("1.");
			printf("********seqNo: %d, base: %d, pck_rcvd_rwnd: %d, swnd: %d, no_unAck: %d\n", seqNo, base, pck_rcvd_rwnd, swnd, no_unAck);
			while (seqNo < base + swnd) {
				if (seqNo == base) {
					init_timer (timer, time_start);
				}
				if (type == DATA)
					temp_payload = dataArray[seqNo - base];
				else if (type == PROBE)
					temp_payload = "Just for probing";
				write_packet(&pck_sent, seqNo, ackNo, swnd, type, temp_payload);
				printf ("\n\n---------------------------\n---send packet: "); 
				print_packet (pck_sent);
				puts (pck_sent.payload);

				if (sendUnreliably(sockfd, &pck_sent, addr, addLen) < 0) {
					perror("send failed");
					exit(0);
				}
				seqNo++;
				//printf("********seqNo: %d, base: %d, pck_rcvd_rwnd: %d\n", seqNo, base, pck_rcvd_rwnd);
			}
		}

		/* Receive ack */
		int recvStatus;
		if (recvStatus = recvfrom(sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
			puts("============================time out=============================");
			printf("---resend time: %d", try);
		}

		//check receive status
		if (recvStatus >= 0) {

			//update base if received appropriate ACK
			if ((pck_rcvd.ackNum >= base)) {//&& (*dataArray[seqNo - base - 1] != '*')
				//cummulative ack technique
				base += (pck_rcvd.ackNum - base + 1);
			}
			if (pck_rcvd.winSize > 0) {
				pck_rcvd_rwnd = pck_rcvd.winSize;
				type = DATA;
				pck_type = "DATA";      // type of packet to be sent out
			}
			else {
				pck_rcvd_rwnd = 1;
				type = PROBE;
				pck_type = "PROBE"; // type of packet to be sent out
			}

			printf ("\n\n---------------------------\n+++receive packet: "); 
			print_packet (pck_rcvd);
			
			
			if (seqNo == base) 
				stop_timer (timer);
			else 
				init_timer (timer, time_start);
		}
		//puts ("!!!");
		//terminate if end of file and received all the packets
		if (((seqNo > base) && (*dataArray[seqNo - base - 1] == '*')) || (*dataArray[seqNo - base] == '*') && (pck_rcvd.ackNum == seqNo - 1)) {
			ser_terminate (sockfd, addr, addLen);
			break;
		}			
		//puts ("@@@");
	}
	fclose(rq_file_fp);
	puts ("Finish send data to client");
	close(sockfd);
	
}
static void ser_terminate(int sockfd, struct sockaddr *addr, int addLen) {//, pckStruct_t pck_sent, pckStruct_t pck_rcvd) {

	uint32_t seqNo, ackNo, win, flag;
    int len;
    char buf[496];
    pckStruct_t pck_sent;
	pckStruct_t pck_rcvd;

	struct itimerval *timer;
    sigset_t timer_signal_set;
    char *pck_type;

	/* Running timers to keep track of lost packets */
    //signal (SIGALRM, signal_hello_bye_function); // use signal_hello_function function to handle SIGALRM
    //sigemptyset (&timer_signal_set);
    //sigaddset (&timer_signal_set, SIGALRM);

    timer = (struct itimerval *) malloc (sizeof (struct itimerval));
    int time_start = TIMER_ACK;

    puts("end of file. Prepare to terminate...");

 send_term_fin:
	//Step 2: send FIN
	
	pck_type = "FIN";
	setjmp (normal_buf);
	puts("\n\n\n---------------------------\n---send FIN");
	init_timer (timer, time_start);
	write_packet (&pck_sent, 0, 0, swnd, FIN, "Hi, end of file, finish sending data");
	
	/* Check if SYN_ACK is lost? */
	//if (is_packet_loss (time_start, pck_type)) {
		//goto time_out;
	//}
	if (sendto (sockfd, &pck_sent, sizeof(pck_sent), 0, addr, addLen) < 0) {
		perror ("send failed");
		exit (1);
	}

 time_out: /* Read note in cli_hand_shake function, inside client program */
	time_out_handler (PEER_HOST, pck_type);

 receive_term_ack:	
	//Step 3: receive FIN_ACK
	sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);
        
	if (recvfrom (sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
		perror ("recv failed");
		exit (1);
	}	

	read_packet (pck_rcvd, &seqNo, &ackNo, &win, &flag, buf);
	sigprocmask (SIG_BLOCK, &timer_signal_set, NULL);

	if (pck_rcvd.type == FIN_ACK) {
		puts ("\n\n\n---------------------------\n+++received FIN_ACK. \n---Terminate successful!");
	}
	else
		goto receive_term_ack;
	stop_timer (timer);
    sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);

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