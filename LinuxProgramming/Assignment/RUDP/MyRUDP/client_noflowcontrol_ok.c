#include "client.h"
#include "util.h"


/* FUNCTION PROTOTYPE */
static void signal_hello_function(int sigNum);

static int read_client_params ();
static int init_client (); //struct sockaddr_in *ipServer, struct in_addr client_addr);
/*
#include <sys/types.h>          
#include <sys/socket.h>
int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
*/
static int check_server_on_same_network (char *client);
/*
 In C, a static function is not visible outside of its translation unit, which is the object file it is compiled into. In other words, making a function static limits its scope.
*/

static void cli_hand_shake (int sockfd, struct sockaddr *addr, int addLen);//, pckStruct_t pck_sent, pckStruct_t pck_rcvd);

/* receive file from server */
static void cli_rudp_handler (int sockfd, struct sockaddr *addr, int addLen);

/* write data into downloaded file */
//static void *write_file (void *arguments);
static void write_file (char *wr_data);

/* FUNCTION DECLARATION */

/* if a specific signal is catched, the program will perform this function */
static void signal_hello_function(int sigNum){
    siglongjmp (hello_signal, 1); // return to the location which stored in hello_signal buffer
}

/*
#include <strings.h>

void bzero(void *s, size_t n); 

The bzero() function shall place n zero-valued bytes in the area pointed to by s
*/

/*
* read_client_params
* 
* Read parameters from file: client.in
*/

static int read_client_params () {
	FILE *fp;
	int lineNo = 1;
	size_t len;
	char buf[MAXSIZE];

	fp = fopen (CLIENT_INPUT, "r");
	if (!fp) {
		return -1;
	}

	/* Read the client parameters one line at a time */
	while (fgets(buf, MAXSIZE, fp)) {
		switch (lineNo) {
			case 1: 
				strncpy (serverIp, buf, INET_ADDRSTRLEN);
				break;	
			case 2: 
				serverPort = atoi (buf);
				if (serverPort == 0)
					return -1;
				break;	
			case 3: 
				strncpy (dld_file_name, buf, MAX_FILENAME_LEN);
				break;	
			case 4: 
				rwnd = atoi (buf);
				if (rwnd == 0)
					return -1;
				break;	
			/*case 5: 
				randomSeed = atoi (buf);
				if (randomSeed == 0)
					return -1;
				break;	
			case 6: 
				lossProbability = atof (buf);
				break;	*/
			default: 
				expDistributor = atof (buf);
				break;	
			
		}
		lineNo ++;
		bzero (buf, MAXSIZE);
	}

	printf("CLIENT PARAMS:\n");
	printf("Server Ip: %s\n", serverIp);
	printf("Server Port: %d\n", serverPort);
	printf("Downloaded file: %s\n", dld_file_name);
	printf("rwnd: %d\n", rwnd);
	printf("Random seed: %d\n", randomSeed);
	printf("Loss Probability: %.2f\n", lossProbability);
	printf("Exp Distributor: %.2f\n", expDistributor);

	return 0;
}

/*
 * init_client
 *
 * Create a socket to communicate with the server
 */
static int init_client () {
	int sockfd, buf_size = BUF_SIZE;

	/* Create the socket */
	sockfd = socket (AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
		perror ("Failed to create socket on the client.\n");

	/* Set buffer size */
	//buf_size = 1024 * 1024;
	if (setsockopt (sockfd, SOL_SOCKET, SO_RCVBUF, &buf_size, sizeof (buf_size)) < 0)
		perror ("Failed to set recv buf\n");

	if (setsockopt (sockfd, SOL_SOCKET, SO_SNDBUF, &buf_size, sizeof (buf_size)) < 0)
		perror ("Failed to set send buf\n");
	return sockfd;
}

/*
* Three way handshake
*
*/
static void cli_hand_shake(int sockfd, struct sockaddr *addr, int addLen) {

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


send_first_hello:	
	
	/* Step 1: send first syn signal (SYN) to server */
	pck_type = "SYN";
	setjmp (normal_buf);
	puts("\n\n\n---------------------------\n---send SYN");
	init_timer (timer, time_start);
	write_packet(&pck_sent, 0, 0, rwnd, SYN, "FIRST HANDSHAKE: SYN");
	// ??? send_Packet function

	/* Check if SYN is lost? */
	if (is_packet_loss(time_start, pck_type)) {
		goto time_out;
	}
	if (sendto(sockfd, &pck_sent, sizeof(pck_sent), 0, addr, addLen) < 0) {
		perror("send failed");
		exit(1);
	}

time_out: /* Read note below */
	time_out_handler (PEER_HOST, pck_type);

receive_second_hello:	
	/* Step 2: receive SYN_ACK */
	/* Waiting for secnd handshake */
	/* block SIGALRM so that we don't end up in an inconsistent state */
	sigprocmask(SIG_UNBLOCK, &timer_signal_set, NULL);
	/* Note!!!
	The first time, the program goes to label: time_out, will go to here, 
	but don't receive any packet -> SIGALARM is catched -> perform
	signal handler (signal_hello_function) -> siglongjmp is performed
	-> the program location is restored -> program jump to label: time_out
	-> 'if sigsetjmp' condition is true (restored from siglongjmp).
	-> 'time out' condition is performed (note that after resending
	program go to label time_out without siglongjmp -> 2nd 'if sigsetjmp' 
	condition is false)
	*/
	//len = pck_rcvdet(sockfd,&packet,DATAGRAM_LEN,"Second Hand-Shake");
	if (recvfrom(sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
		perror("recv failed");
		exit(1);
	}
	read_packet (pck_rcvd, &seqNo, &ackNo, &win, &flag, buf);
	printf("\n\n\n---------------------------\n+++received: %s\n", buf);
	sigprocmask(SIG_BLOCK, &timer_signal_set, NULL);

	/* */
    stop_timer (timer);
    sigprocmask(SIG_UNBLOCK, &timer_signal_set, NULL);

send_third_hello:	
	//Step 3: send SYN_ACK_ACK
	if (pck_rcvd.type == SYN_ACK) {
		pck_type = "SYN_ACK_ACK";
		setjmp (normal_buf);
		puts("\n\n\n---------------------------\n---send SYN_ACK_ACK");
		init_timer (timer, time_start);
		write_packet(&pck_sent, 0, 0, 0, SYN_ACK_ACK, "THIRD HANDSHAKE: SYN_ACK_ACK");
		// ??? send_Packet function

		if (is_packet_loss(time_start, "SYN_ACK_ACK")) {
			puts ("resend third hello!");
			goto time_out_2;
		}
		if (sendto(sockfd, &pck_sent, sizeof(pck_sent), 0, addr, addLen) < 0) {
			perror("send failed");
			exit(1);
		}
		time_out_2: /* Read note in send_first_hello lable for understanding */
			time_out_handler (PEER_HOST, pck_type);
			//stop_timer (timer);
		    //sigprocmask(SIG_UNBLOCK, &timer_signal_set, NULL);

	}
	sigprocmask(SIG_UNBLOCK, &timer_signal_set, NULL);
	/* We wait and expect to receive DATA packet, but unfortunately it is able to receive
	   SYN_ACK from server 
	   -> Although receive any thing, but it indicates that SYN_ACK_ACK is sent successfully
	*/
	if (recvfrom(sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
		perror("recv failed");
		exit(1);
	}
    
	read_packet (pck_rcvd, &seqNo, &ackNo, &win, &flag, buf);
	sigprocmask(SIG_BLOCK, &timer_signal_set, NULL);
	if (flag == DATA) {
		puts("---------------------------\n+++prepare receive data from the server...");	
		puts ("\n\n\n---------------------------\n");
		print_packet (pck_rcvd);
		write_file (pck_rcvd.payload );
	}
	else
		puts("---------------------------\n+++prepare receive data from the server...");	
	stop_timer (timer);
    sigprocmask (SIG_UNBLOCK, &timer_signal_set, NULL);
}

static void cli_rudp_handler (int sockfd, struct sockaddr *addr, int addLen) {
	
	FILE *dld_file_fp; /* pointer of dowloaded file */
	uint32_t seqNo, ackNo, win, flag;
    int len;
    char buf[PAYLOAD];
    pckStruct_t pck_sent;
	pckStruct_t pck_rcvd;
	seqNo = 1, ackNo = 1;

	struct itimerval *timer;
    sigset_t timer_signal_set;
    char *pck_type;

    dld_file_fp = fopen (DOWLOADED_FILE, "a"); // if we use dld_file_name -> get error: Segmentation fault (core dumped)

    if (!dld_file_fp) {
    	exit (1);
    }

    //puts("\n\n\n---------------------------\n+++prepare receive data to the client...");

    //begin receiving data
	while (1) {

		//receive packet
		if (recvfrom(sockfd, &pck_rcvd, sizeof(pck_rcvd), 0, addr, &addLen) < 0) {
			perror("recv failed");
			exit(1);
		}
		printf("\n\n---------------------------\n+++receive packet: ");
		print_packet (pck_rcvd);
		fputs(pck_rcvd.payload, dld_file_fp);

		//send ACK
		write_packet(&pck_sent, seqNo, ackNo ++, rwnd, ACK, buf);
		printf ("\n\n---------------------------\n---send packet: "); 
		print_packet (pck_sent);

		if (sendto(sockfd, &pck_sent, sizeof(pck_sent), 0, addr, addLen) < 0) {
			perror("send failed");
			exit(1);
		}
		puts(pck_rcvd.payload);
		if (*(pck_rcvd.payload) == '*') {
			break;
		}
	}
	fclose(dld_file_fp);
	puts ("Finish receive data from server");
	close(sockfd);
}

//static void *write_file (void *arguments) {
static void write_file (char *wr_data) {

	//wr_argument_t *args = (wr_argument_t *) arguments;
	
	//FILE *file_fp = args -> fp;
	//char *wr_data = args -> data;

	FILE *file_fp;
	file_fp = fopen (DOWLOADED_FILE, "w");
    if (!file_fp) {
    	puts ("Error: cannot open file to write.");
    	exit (1);
    }
    fputs (wr_data, file_fp);
    fclose (file_fp);
}

int main(int argc, char const *argv[])
{
	int sockfd, addr_len;
	//struct in_addr *serv_addr;
	//struct in_addr client_addr;
	struct sockaddr_in server_addr;
	//char addr[INET_ADDRSTRLEN];
	//pthread_t write_file_thread, download_file_thread;
	

	pckStruct_t pck_sent;
	pckStruct_t pck_rcvd;

	/* Get the parmeters from client.in */
	read_client_params ();

	server_addr.sin_family      = AF_INET;
	server_addr.sin_port 		= htons(serverPort);
	server_addr.sin_addr.s_addr = inet_addr (serverIp);

	sockfd = init_client (); 

	addr_len = sizeof (server_addr);
	cli_hand_shake (sockfd, (struct sockaddr *) &server_addr, addr_len);
	cli_rudp_handler (sockfd, (struct sockaddr *) &server_addr, addr_len);
	
	return 0;
}