#include "util.h"
#define MAX_SIZE 256
#include <string.h>
#include <time.h>
#include "stdlib.h"
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include "unpifiplus.h"
#include <string.h>
#include <errno.h>
#include <stddef.h>
#include "util.h"
#include "unprtt.h"
#include <setjmp.h>
#include <errno.h>
static char serverip[MAX_SIZE];
static char servport[MAX_SIZE];
static char reqfile[MAX_SIZE];
static int cliWindow;
int seed;
float lossProbability;
int exp_distributor;
int connect_err = 0;
static char getVal[MAX_SIZE];

sigjmp_buf hello_signal;

static void signal_hello_function(int sigNum){
        siglongjmp(hello_signal,1);

}

//Retrive client parameters
static void get_client_parameters(){
        FILE *fp;

        fp=fopen("client.in","r");

   	if(fp==NULL) {
           printf("Error in opening file");
           exit(0);
   	}

        rewind(fp);
        fgets(serverip,INET_ADDRSTRLEN,fp);
	char *temp;
	temp = strchr(serverip,'\n');
	*temp = '\0';

        fgets(servport,256,fp);
	temp = strchr(servport,'\n');
	*temp = '\0'; 

        fgets(reqfile,256,fp);
	temp = strchr(reqfile,'\n');
	*temp = '\0';
	temp = strchr(reqfile,' ');
	if (temp !=  NULL)
		*temp = '\0';

	fgets(getVal,256,fp);
        temp = strchr(getVal,'\n');
	if (temp != NULL)
        	*temp = '\0';
	cliWindow = atoi(getVal);

	memset(getVal,'\0',256);
        fgets(getVal,256,fp);    
        temp = strchr(getVal,'\n');
	if (temp != NULL)
        	*temp = '\0';
        seed = atoi(getVal);
	//srand(seed);

        memset(getVal,'\0',256);
        fgets(getVal,256,fp);    
        temp = strchr(getVal,'\n');
	if (temp != NULL)
        	*temp = '\0';
        lossProbability = atof(getVal);
	
	//fetching exponential distributor
        memset(getVal,'\0',256);
    	fgets(getVal,256,fp);
    	temp = strchr(getVal,'\n');
        if (temp != NULL)
                *temp = '\0';
    	exp_distributor = atoi(getVal);


        Fclose(fp);
        //printf("\n details are %s,%s,%s %d length is %d \n",serverip,servport,reqfile,cliWindow);

}

//Is packet Lost
int isPacketLost()
{  
  //        srand(seed);
	//srand48(time(NULL));
	//return 0;
	 static int index = 0;
	
	float arry[100] = {0.01, 0.35, 0.24, 0.34, 0.27, 0.08, 0.01, 0.93, 0.41, 0.3, 0.93, 0.52, 0.37, 0.33, 0.24, 0.21, 0.67, 0.89, 0.2, 0.05, 0.92, 0.45, 0.33, 0.82, 0.86, 0.87, 0.35, 0.27, 0.28, 0.34, 0.48, 0.95, 0.49, 0.39, 0.76, 0.96, 0.28, 0.12, 0.23, 0.86, 0.59, 0.84, 0.94, 0.87, 0.64, 0.59, 0.24, 0.82, 0.33, 0.93, 0.32, 0.98, 0.66, 0.29, 0.7, 0.53, 0.73, 0.88, 0.04, 0.4, 0.51, 0.4, 0.98, 0.59, 0.6, 0.44, 0.97, 0.65, 0.24, 0.76, 0.51, 0.36, 0.62, 0.24, 0.94, 0.66, 0.68, 0.69, 0.25, 0.41, 0.22, 0.92, 0.5, 0.07, 0.76, 0.93, 0.72, 0.95, 0.18, 0.64, 0.69, 0.42, 0.23, 0.3, 0.49, 0.43, 0.12, 0.59, 0.5, 0.97};
	float randomP = arry[index++];
	if (index > 99)
		index = 0;
        //float randomP= (float) rand() / (float) RAND_MAX;
        if ( randomP < lossProbability)
        {       //printf("\n value of randomP is %f",randomP);
                printf(" \n Packet Lost! \n");
                return 1;
        } else {
		 //printf("\n value of randomP is %f",randomP);
                return 0;
	} 

}

//Get the host name and address
struct in_addr *gethost(char *host, char *addr){
        struct hostent *host_detail = NULL;
        struct in_addr host_ip;
	struct in_addr *host_ipv;
	host_ipv = &host_ip;

        int ret = inet_pton(AF_INET,host,&host_ip);
        if (ret > 0){
                host_detail = gethostbyaddr(&host_ip,sizeof(host_ip),AF_INET);
        } else {
                host_detail = gethostbyname(host);
        }
        if (host_detail == NULL){
		  printf("Server IP does not exists!");
                  exit(0);
                //return NULL;
        } else {
                if (inet_ntop(AF_INET,host_detail->h_addr,addr,INET_ADDRSTRLEN) > 0) {
                        printf("Connecting to server %s with ip address %s",host_detail->h_name,addr);
                        return (struct in_addr *)host_detail->h_addr;
                } else {
                        return NULL;
                }
        }
}

//Check server and assign client address
static int check_server_assign_client(struct in_addr *ipserver,struct in_addr *ipclient) {
	struct ifi_info *ifihead = Get_ifi_info_plus(AF_INET,1);
	
	int count = print_interfaces(ifihead);

	if (count == 0)
		return -1;
	
	int flag_dont_route  = is_local(ifihead,ipclient,ipserver);

	free_ifi_info_plus(ifihead);

	return flag_dont_route;
}

//Initializing client
static int initialize_client(struct sockaddr_in *ipserver,struct in_addr client_addr,int flag_dont_route) {
	
	struct sockaddr_in ipclient;
	int sockfd;
	int len;
	char addr_buf[INET_ADDRSTRLEN];     
	int status = 1;
	
	if((sockfd=socket(AF_INET,SOCK_DGRAM,0)) < 0){
                perror("socket:");
                exit(1);
        }
	pack_struct packet;
	packet.flag =SYN;
        if (flag_dont_route == 1){
                Setsockopt(sockfd,SOL_SOCKET,SO_DONTROUTE,&status,sizeof(status));
		//printf("\n flag dont route %d \n",flag_dont_route);
	}

/*
        len = sizeof(ipclient);
	
	memset(&ipclient, 0, sizeof(ipclient));
        ipclient.sin_family = AF_INET;
        ipclient.sin_port   = htons(0);
	ipclient.sin_addr   = client_addr; 

        int bind_status = bind(sockfd, (struct sockaddr *)&ipclient,len);
        if (bind_status < 0){
                perror("Error in binding");
                return -1;
        } 

	Getsockname(sockfd,(struct sockaddr *)&ipclient,&len);
	 printf("\n Address of client is %s and Port no is %d \n",
			inet_ntop(AF_INET,&ipclient.sin_addr,addr_buf,INET_ADDRSTRLEN),
			ntohs(ipclient.sin_port)); 

	len = sizeof(struct sockaddr_in);
	      printf("\n inside write packet_clientmain \n");
	strcpy(packet.payload,"sample.txt"); 

	int c = connect(sockfd,(struct sockaddr *)ipserver,len);
	if (c < 0)
		printf("\n Connect error %s",strerror(errno));
	int g = getpeername(sockfd,(struct sockaddr *)ipserver,&len);

	printf("\n Address of server is %s and Port no is %d \n",
                        inet_ntop(AF_INET,&ipserver->sin_addr,addr_buf,INET_ADDRSTRLEN),
                        ntohs(ipserver->sin_port));

									*/


	//initialize_tcb();

	return sockfd;
}

//Exchanging Hello with Server
int exchangeHello(int sockfd,struct sockaddr_in ipserver,int cliWindow,tcb *clitcb,struct in_addr client_addr){
	pack_struct packet;
	uint32_t seqNo,ackNo,win;
	uint32_t flag;
	int connPort,len;
	char buf[496];
        struct sockaddr_in ipclient;
        char addr_buf[INET_ADDRSTRLEN];
        struct itimerval *timer;
        sigset_t timer_signal_set;

	//Running timers to keep track of lost packets
        Signal(SIGALRM,signal_hello_function);
       sigemptyset(&timer_signal_set);
        sigaddset(&timer_signal_set,SIGALRM);

        timer = (struct itimerval *)Malloc(sizeof(struct itimerval));
        long int time_start = TIMER_FIN;
        //time_start = 0;
        timer->it_interval.tv_sec = 0;
        timer->it_interval.tv_usec = 0;
        timer->it_value.tv_sec = time_start/1000;
        timer->it_value.tv_usec = (time_start % 1000)*1000;

        int hello_retransmits = 0;


send_first_hello:
	seqNo = 1;
	ackNo = 1;
	win = cliWindow;
	flag = SYN;
	
	initialize_timer(timer,time_start);
	writePacket(&packet,seqNo,ackNo,win,flag,reqfile,strlen(reqfile));
//	sendPacket(sockfd,&packet,DATAGRAM_LEN,"First Handshake");
	Sendto(sockfd, &packet, sizeof(pack_struct), 0, (struct sockaddr *)&ipserver, sizeof(ipserver));
	printf("\n Sending First Handshake ");
	               if (sigsetjmp(hello_signal,1) != 0){
                        hello_retransmits++;
                        printf(" \n HELLO Timeout occurred ");
                        if (hello_retransmits >= RETRANS_MAX){
                                printf("\n Server Down, not sending ACK");
                                return;
                        }
                goto send_first_hello;

        }


// Waiting for secnd handshake        
	sigprocmask(SIG_UNBLOCK,&timer_signal_set,NULL);
	len = recvPacket(sockfd,&packet,DATAGRAM_LEN,"Second Hand-Shake");
	readPacket(&packet,len,&seqNo,&ackNo,&win,&flag,buf);
	sigprocmask(SIG_BLOCK,&timer_signal_set,NULL);

        initialize_timer(timer,0);
        sigprocmask(SIG_UNBLOCK,&timer_signal_set,NULL);

	connPort = atoi(buf);
	len = sizeof(ipclient);


	ipserver.sin_port = htons(connPort);

	Connect(sockfd,(struct sockaddr *)&ipserver,sizeof(ipserver));
        //for testing
        len = sizeof(ipclient);
        Getsockname(sockfd,(struct sockaddr *)&ipclient,&len);
        printf("\n after port change Address of client is %s and Port no is %d \n",
                        inet_ntop(AF_INET,&ipclient.sin_addr,addr_buf,INET_ADDRSTRLEN),
                        ntohs(ipclient.sin_port));
        //for testing
        len = sizeof(ipserver);
        int g = getpeername(sockfd,(struct sockaddr *)&ipserver,&len);
        printf("\n  after port change : Address of server is %s and Port no is %d \n",
                        inet_ntop(AF_INET,&(ipserver.sin_addr),addr_buf,INET_ADDRSTRLEN),
                        ntohs(ipserver.sin_port));


send_third_hello:
	flag = SYN_ACK_ACK;
	seqNo = 1;ackNo = 1;
	
        writePacket(&packet,seqNo,ackNo,win,flag,NULL,0);
       	sendPacket(sockfd,&packet,HEADER_SIZE,"Third Handshake");

	return 1;
}


int main() {
	struct in_addr *serv_addr;
	struct in_addr client_addr;
	struct sockaddr_in ipserver;
	char addr[INET_ADDRSTRLEN];
	int sockfd,len;
	
	//Get the parmeters from client.in
	get_client_parameters();

	serv_addr = (struct in_addr *) gethost(serverip,addr);

	//Check for locality
	int flag_dont_route  = check_server_assign_client(serv_addr,&client_addr);

	if (flag_dont_route < 0)
		perror("\n No Valid Interface \n");

        memset(&ipserver, 0, sizeof(ipserver));
        ipserver.sin_family = AF_INET;
        ipserver.sin_port   = htons(atoi(servport));
	ipserver.sin_addr   = *serv_addr;
	printf("\n Address of server is %s and Port no is %d \n",
                        inet_ntop(AF_INET,&ipserver.sin_addr,addr,INET_ADDRSTRLEN),
                        ntohs(ipserver.sin_port));

	//initializing client
	sockfd  = initialize_client(&ipserver,client_addr,flag_dont_route);
	tcb *clitcb = (tcb *)malloc(sizeof(tcb));
	//Client TCB	
	initilaize_client_tcb(clitcb,cliWindow,cliWindow);
	exchangeHello(sockfd,ipserver,cliWindow,clitcb,client_addr);

	// receiveFile(&sockfd,cliWindow,clitcb);
	receive_file(&sockfd,cliWindow,clitcb);

	return 0;
}
