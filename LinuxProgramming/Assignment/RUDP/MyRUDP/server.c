#include "server.h"
#include "util.h"
#include <sys/stat.h>
#include <fcntl.h> // for open() function
#include <unistd.h> // for close() function

static int retransmit_num = 0;
int is_retrans_pck = 0, packetCount = 0;
/* Function prototype of server program 
   For more the details, you can see in client program because of the sames
*/
static void signal_hello_bye_function();
//static int updateDataArray(int base, int seqNo, int ackNo, FILE *in, int type, char dataArray[SENDBUFFER][PAYLOAD], int rwnd);
static void updateDataArray(int base, FILE *in, char dataArray[SENDBUFFER][PAYLOAD]);
static int read_server_params ();
static int init_server ();//struct sockaddr_in server_addr);
static void ser_hand_shake(int sockfd, struct sockaddr *addr, int addLen);//, pckStruct_t pck_sent, pckStruct_t pck_rcvd);
static void ser_rudp_handler (int sockfd, struct sockaddr *addr, int addLen);
static void ser_terminate(int sockfd, struct sockaddr *addr, int addLen);//, pckStruct_t pck_sent, pckStruct_t pck_rcvd);


/* FUNCTION DECLARATION */

static void signal_hello_bye_function() {
    siglongjmp (hello_bye_signal, 1);
}
/* dataArray store the packets which are not sent successfully to the client, and packets which are stored in buffer and will be sent in the next rounds */
/*static int updateDataArray(int base, int seqNo, int ackNo, FILE *in, int type, char dataArray[SENDBUFFER][PAYLOAD], int rwnd) {
	//shift data in TCP based on BASE and SEQNO
	int i;
	int no_unAck = seqNo - base;
	int mark;
	char temp[PAYLOAD];
	static int old_rwnd = WINSIZE;

	//clear dataArray if base = seqNo
	if (no_unAck == 0 && base == 1) {
		for (i = 0; i < SENDBUFFER; i++) { 
			memset(temp, '\0', PAYLOAD);
			fgets(temp, PAYLOAD, in);
			strcpy(dataArray[i], temp);
		}
	} 
	else if (type == DATA) {
		//if (ackNo > base)
			mark = old_rwnd - no_unAck;
		//else
		//	mark = 0;
		printf("\n~~~old_rwnd = %d\tno_unack = %d\n", old_rwnd, no_unAck);
		//if (mark > 1)
		//	mark = 1;
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
	for (i = 0; i < SENDBUFFER; ++i) 
		printf("a[%d] = %s", i, dataArray[i]);
	
	old_rwnd = rwnd;
	return no_unAck;
}*/

static void updateDataArray(int base, FILE *in, char dataArray[SENDBUFFER][PAYLOAD]) { //, int recvWinSize) {
	//shift data in TCP based on BASE and SEQNO
	int i;
	int distance = packetCount + 1 - base;
	//clear dataArray if base = seqNo
	if (distance == 0) {
		memset(dataArray, 0, sizeof(dataArray));
	} else {
		for (i = 0; i < distance; i++) {
			strcpy(dataArray[i], dataArray[i + SENDBUFFER - distance]);
		}
	}

	//clear Window from distance
	char temp[PAYLOAD];
	for (i = distance; i < SENDBUFFER; i++) {
		strcpy(dataArray[i], "");
	}
	//insert data
	for (i = distance; i < SENDBUFFER; i++) {
		memset(temp, '\0', PAYLOAD);
		fgets(temp, PAYLOAD, in);
		strcpy(dataArray[i], temp);
		packetCount++;
	}
	for (i = 0; i < SENDBUFFER; ++i) 
		printf("a[%d] = %s", i, dataArray[i]);
	
}
static int read_server_params () {
	FILE *fp;
	int lineNo = 1;
	char buf[MAXSIZE];

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
	}
	printf("SERVER PARAMS:\n");
	printf("Server Port: %d\n", serverPort);
	printf("swnd: %d\n", swnd);
	printf("Requested file: %s\n", req_file_name);
	return 0;
}

static int init_server () {
	int sockfd, buf_size, optval = 1;
	struct sockaddr_in server_addr;
	buf_size = BUF_SIZE;

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
	
	if (bind (sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0) {
		perror("bind failed");
		exit(1);
	}	
	printf("Server bind successfully\nWaiting for incomming connection...\n");
	return sockfd;
	
}

static void ser_hand_shake(int sockfd, struct sockaddr *addr, socklen_t  addLen) { //, pckStruct_t pck_sent, pckStruct_t pck_rcvd) {

	uint32_t seqNo, ackNo, win, flag;
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

 //receive_first_hello:
	//Step 1: receive first syn signal (SYN) from client
	if (recvfrom(sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
		perror("recv failed");
		exit(1);
	}
	current_time (&t_current);
	delta_t = get_durration(&t_base, &t_current) / 1000.0;
	retransmit_num = 0;
	read_packet (pck_rcvd, &seqNo, &ackNo, &win, &flag, buf);
	printf("\n\n\n----------------------------------------------------------\n+++Time: %.4f\treceived: %s", delta_t, buf);

 //send_second_hello:
	//Step 2: send SYN_ACK
	if (pck_rcvd.type == SYN) {
		pck_type = "SYN_ACK";
		setjmp (normal_buf);
		current_time (&t_current);
		delta_t = get_durration(&t_base, &t_current) / 1000.0;
		printf ("\n\n\n----------------------------------------------------------\n---Time: %.4f\tsend SYN_ACK\n", delta_t); 
		make_rtt (RTT);
		init_timer (timer, time_start);
		write_packet(&pck_sent, 0, 0, swnd, SYN_ACK, "SECOND HANDSHAKE: SYN_ACK");
		
		/* Check if SYN_ACK is lost? */
		if (!is_packet_loss(time_start, pck_type)) {
			if (sendto(sockfd, &pck_sent, sizeof(pck_sent), 0, addr, addLen) < 0) {
				perror("send failed");
				exit(1);
			}
		}
	}
 //time_out: /* Read note in cli_hand_shake function, inside client program */
	time_out_handler (&retransmit_num, PEER_HOST, pck_type);

 receive_third_hello:	
	//Step 3: receive SYN_ACK_ACK
	sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);
        
	//pck_rcvdet(sockfd,packet,DATAGRAM_LEN,"Third Hand-Shake");
	if (recvfrom (sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
		perror ("recv failed");
		exit (1);
	}	
	current_time (&t_current);
	delta_t = get_durration(&t_base, &t_current) / 1000.0;
	retransmit_num = 0;

	read_packet (pck_rcvd, &seqNo, &ackNo, &win, &flag, buf);
	sigprocmask (SIG_BLOCK, &timer_signal_set, NULL);

	if (pck_rcvd.type == SYN_ACK_ACK) {
		printf ("\n\n\n----------------------------------------------------------\n+++Time: %.4f\treceived SYN_ACK_ACK. \n---Finish handshaking!\n", delta_t);
	}
	else
		goto receive_third_hello;
	stop_timer (timer);
    sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);
}

static void ser_rudp_handler (int sockfd, struct sockaddr *addr, socklen_t  addLen) {
	
	FILE *rq_file_fp; /* pointer of requested file */
	uint32_t seqNo, ackNo, base, no_unAck;
	char dataArray[SENDBUFFER][PAYLOAD];
    uint32_t type, i, j = 1; 
	int recvStatus;
	int count_duplicate = 1;
    
    char *pck_type, *temp_payload; // type of packet to be sent out, which is used to printing to the screen 
    pckStruct_t pck_sent;
	pckStruct_t pck_rcvd;
	seqNo = 1, ackNo = 1; 
	base = 1;

	struct itimerval *timer;
    sigset_t timer_signal_set;
    
	/* Running timers to keep track of lost packets */
    signal (SIGALRM, signal_hello_bye_function); // use signal_hello_function function to handle SIGALRM
    sigemptyset (&timer_signal_set);
    sigaddset (&timer_signal_set, SIGALRM);

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
    	if (is_retrans_pck == 0) {
	    	if (pck_rcvd_rwnd > no_unAck) {
	    		swnd = pck_rcvd_rwnd - no_unAck;
	    	}
	    	else
	    		swnd = 0;
	    }
	    else
	    	swnd = pck_rcvd_rwnd;

    	printf("--------isTimeout = %d\n", isTimeout);
    	//send_data:

			make_rtt (RTT);
    		setjmp (normal_buf);	
    		if (isTimeout == 1) {
    			init_timer (timer, time_start);
				isTimeout = 0;
				printf("===Resend from seqNo: %d \n", base);

				for (i = 0; i < pck_rcvd_rwnd; i++) {
					if (type == DATA)
						temp_payload = dataArray[i];
					else if (type == PROBE)
						temp_payload = "Just for probing";
					write_packet(&pck_sent, base + i, ackNo, swnd, type, temp_payload);
					current_time (&t_current);
					delta_t = get_durration(&t_base, &t_current) / 1000.0;
					
					printf("\n\n----------------------------------------------------------\n");
					printf("---send packet: %d\n", base + i);
					printf("---Time: %.4f\tsend packet: ", delta_t);
					print_packet (pck_sent);

					if (!is_packet_loss(time_start, pck_type)) {
						if (sendto(sockfd, &pck_sent, sizeof(pck_sent), 0, addr, addLen) < 0) {
							perror("send failed");
							exit(1);
						}
					}
					if (!strcmp (temp_payload, "*"))
						break;
				}
			}
			else if (isTimeout == 0) {
				j = seqNo;
				if (is_retrans_pck && (pck_rcvd.ackNum < seqNo)) {
					j = base;
					is_retrans_pck = 0;
				}
				updateDataArray(base, rq_file_fp, dataArray);
				printf("\nj = %d, seqNo = %d, base = %d, rwnd = %d\n", j, seqNo, base, pck_rcvd_rwnd);

				//send the next seqNo
				while (j < base + pck_rcvd_rwnd) { // WARNING: differ between '' AND ""
					if (!strcmp (dataArray[j - base], "")) {
						break;
						isTimeout = 2;
					}
					if (j == base) {
						init_timer (timer, time_start);
					}
					if (type == DATA)
						temp_payload = dataArray[j - base];
					else if (type == PROBE)
						temp_payload = "Just for probing";
					write_packet(&pck_sent, j, ackNo, swnd, type, temp_payload);
					current_time (&t_current);
					
					printf("\n\n----------------------------------------------------------\n");
					printf("---Time: %.4f\tsend packet: ", delta_t);
					print_packet (pck_sent);
					
					if (!is_packet_loss(time_start, pck_type)) {
						if (sendto(sockfd, &pck_sent, sizeof(pck_sent), 0, addr, addLen) < 0) {
							perror("send failed");
							exit(1);
						}
					}
					j ++;
					if (seqNo < j)
						seqNo++;
				}
				//printf("********seqNo: %d, base: %d, pck_rcvd_rwnd: %d, swnd: %d, no_unAck: %d\n", seqNo, base, pck_rcvd_rwnd, swnd, no_unAck);
			}
		//time_out:
			if (isTimeout != 2)
				isTimeout = 1;
			stop_timer (timer);
			init_timer (timer, time_start);
			time_out_handler (&retransmit_num, PEER_HOST, pck_type);

		//receive_ack:
			/* Receive ack */
			sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);
			if (recvStatus = recvfrom(sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
				perror ("recv failed");
				exit (1);
			}
			no_unAck = seqNo - base;
			current_time (&t_current);
			delta_t = get_durration(&t_base, &t_current) / 1000.0;
			retransmit_num = 0;
			sigprocmask (SIG_BLOCK, &timer_signal_set, NULL);
			if (isTimeout != 2)
				isTimeout = 0;
			//check receive status
			if (recvStatus >= 0) {

				printf ("\n\n----------------------------------------------------------\n+++Time: %.4f\treceive packet: ", delta_t); 
				print_packet (pck_rcvd);
				//update base if receive appropriate ACK
				if ((pck_rcvd.ackNum > base)) {
					//cummulative ack technique
					base += (pck_rcvd.ackNum - base + 1);
				}
				else { 
					/* a duplicate ACK for already ACKed segment */
					count_duplicate ++;
					if (count_duplicate == 3) {
						count_duplicate = 1;
						isTimeout = 0;
						stop_timer (timer);
						is_retrans_pck = 1;
						printf("Fast retransmit!\n");
					}
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

				if (seqNo == base) 
					stop_timer (timer);
				else 
					init_timer (timer, time_start);
				sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);
			}
			//terminate if end of file and received all the packets
			printf(".........seqNo: %d, base: %d, pck_rcvd_rwnd: %d, swnd: %d, no_unAck: %d\n", seqNo, base, pck_rcvd_rwnd, swnd, no_unAck);
			if (isTimeout == 2 || ((seqNo > base) && (*dataArray[seqNo - base - 1] == '*')) || ((seqNo >= base) && strcmp (dataArray[seqNo - base], "") == 0)) {  //(*dataArray[seqNo - base] == '*')
				if (pck_rcvd.ackNum == seqNo - 1) {
					sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);
					stop_timer (timer);
					//ser_terminate (sockfd, addr, addLen);
					break;
				}
				else if (seqNo == base)
					isTimeout = 2;
			}	
			
	}
	fclose(rq_file_fp);
	puts ("Finish send data to client");
	close(sockfd);
	
}

static void ser_terminate(int sockfd, struct sockaddr *addr, socklen_t  addLen) {//, pckStruct_t pck_sent, pckStruct_t pck_rcvd) {

	uint32_t seqNo, ackNo, win, flag;
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
    
 //send_term_fin:
	//Step 2: send FIN
	current_time (&t_current);
	delta_t = get_durration(&t_base, &t_current) / 1000.0;
	pck_type = "FIN";
	setjmp (normal_buf);
	printf ("\n\n\n----------------------------------------------------------\n---Time:  %.4f\tsend FIN", delta_t);
	make_rtt (RTT);
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

 //time_out: /* Read note in cli_hand_shake function, inside client program */
	time_out_handler (&retransmit_num, PEER_HOST, pck_type);

 receive_term_ack:	
	//Step 3: receive FIN_ACK
	sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);
        
	if (recvfrom (sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
		perror ("recv failed");
		exit (1);
	}
	retransmit_num = 0;	

	read_packet (pck_rcvd, &seqNo, &ackNo, &win, &flag, buf);
	sigprocmask (SIG_BLOCK, &timer_signal_set, NULL);

	if (pck_rcvd.type == FIN_ACK) {
		puts ("\n\n\n----------------------------------------------------------\n+++received FIN_ACK. \n---Terminate successful!");
	}
	else
		goto receive_term_ack;
	stop_timer (timer);
    sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);

}

int main()
{
	int sockfd;
	socklen_t  addr_len;
	struct sockaddr_in server_addr, client_addr;

	//Get the parmeters from client.in
	read_server_params ();

	sockfd = init_server (); 
	current_time (&t_base);

	addr_len = sizeof (server_addr);
	ser_hand_shake (sockfd, (struct sockaddr *) &client_addr, addr_len);
	ser_rudp_handler (sockfd, (struct sockaddr *) &client_addr, addr_len);
	ser_terminate (sockfd, (struct sockaddr *) &client_addr, addr_len);
	return 0;
}