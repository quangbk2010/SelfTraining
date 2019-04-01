#include <stdio.h>
#include <stdlib.h>
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
#include "server.h"

#define MAXSIZE 256
#define MAX 10
int latest_child = 0;

static int server_port_no;
static int sliding_window_size;

static tcb *conn_q;
struct  pckQueue *queueHead;
static clientRequestList *head;

sigjmp_buf hello_signal;

//handle child
static void child_handler(int sigNum) {
	if (latest_child == 0)
		return;
	deleteClient(latest_child);
	latest_child = 0;

}
static void signal_hello_function(int sigNum){
	siglongjmp(hello_signal,1);

}
void signal_handler(int signalNo)
{
                printf("Error no:%d",signalNo);
                return;
}

//Checking if client exists
int isClientExists(struct sockaddr_in *current,struct sockaddr_in *new)
{
        if((current->sin_addr.s_addr == new->sin_addr.s_addr) ||
        (current->sin_port == new->sin_port) ||
        (current->sin_family == new->sin_family))
                {
                        printf("\nclient already exists!");
                        return 1;
                }

        return 0;
}
//displaying client list
void displayClientList()
{
        clientRequestList *p=head;
        while(p!=NULL)
        { //char buf[ADDR_STRLEN];
        printf(" \n Client info:%s, port:%d child pid : %d\n",Sock_ntop_host((struct sockaddr *)&p->cliaddr, sizeof(struct sockaddr)),p->cliaddr.sin_port,p->pid);

                p=p->next;
        }



}

//deleting client list
int deleteClient(int pid)
{

        clientRequestList *p=head;
	clientRequestList *prev = head;

	if (head == NULL)
		return 0;
        while(p !=NULL)
        {       
                if(p->pid == pid) {
			if ( p == head){
				head = head->next;
			} else {
				prev->next = p->next;
			}
			printf("\n Child deleted \n");
			return 1 ;

		}
	
		prev = p;	
                p=p->next;
		if ( p == NULL)
			break;
        }

        return 0;

}

//checking client 
int checkClient(struct sockaddr_in cliaddr)
{

        if(head==NULL)
        {
	
        return 1;
        }
        clientRequestList *p=head;
        while(p->next!=NULL)
        {
                if(isClientExists(&(p->cliaddr),&cliaddr)==1)
                        return 0;
                p=p->next;
        }

        return 1;

}

//adding client to list
int addClient(struct sockaddr_in cliaddr,pid_t pid)
{

        if(head==NULL)
        {
        clientRequestList *newPck= (clientRequestList *)Malloc(sizeof(clientRequestList));
        newPck->cliaddr=cliaddr;
        newPck->next=NULL;
	newPck->pid = pid;
        head=newPck;
        displayClientList(head);
        return 1;
        }
        clientRequestList *p=head;
        while(p->next!=NULL)
        {
                if(isClientExists(&(p->cliaddr),&cliaddr)==1)
                        return 0;
                p=p->next;
        }

        clientRequestList *newPck= (clientRequestList *)Malloc(sizeof(clientRequestList));
        newPck->cliaddr=cliaddr;
        newPck->next=NULL;
	newPck->pid = pid;
        p->next=newPck;
        displayClientList();
        return 1;

}

//Bind discovered IP address
int* bindDiscoveredIPs(struct ifi_info *ifihead,int totalIPs)
{
	struct sockaddr_in servaddr;
	struct ifi_info *ifi;
	int i, j,family=AF_INET;;
	int *sockfd,b;
	struct sockaddr_in *sa;
	

	sockfd=(int*)Malloc(totalIPs*sizeof(int));
	
	for(i=0,ifi=ifihead;ifi!=NULL;ifi=ifi->ifi_next,i++)
	{        
		bzero(&servaddr,sizeof(servaddr));
		sockfd[i]=Socket(family,SOCK_DGRAM,0);
		servaddr.sin_family = family;
		servaddr = *(struct sockaddr_in *)ifi->ifi_addr;
		servaddr.sin_port = htons(server_port_no);
	
	
	if(bind(sockfd[i],(struct sockaddr *)&servaddr, sizeof(servaddr))<0)
	{
		printf("error in bind");
		perror("bind");
	}
	printf("port:%d",server_port_no);
	sa = (struct sockaddr_in *)ifi->ifi_addr;
	printf("IP:%s",Sock_ntop_host((struct sockaddr *)sa, sizeof(*sa)));
	
	}
	return sockfd;
}

//Exchange Hello packet with client
int exchangeHello(int sockfd,struct sockaddr_in cliaddr,int cliWindow,pack_struct *buf,char *filename,int isLocal){

	pid_t pid;
	
	//child process
	
	//if ((pid = Fork()) == 0) {

	
        pack_struct *packet;
        uint32_t seqNo,ackNo,win;
        uint32_t flag;
        int connPort;
	uint32_t len;
        char *file;
	//char filename[496];
	file = filename;
	packet = buf;
	char temp_buffer[495];
	static tcb sertcb;
	 struct sockaddr_in servAddr;

	struct itimerval *timer;
        sigset_t timer_signal_set;
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


process_first_hello:

        readPacket(buf,len,&seqNo,&ackNo,&win,&flag,file);
	printf("\n First handshake with file request %s \n",file);
	
send_second_hello:
        flag = SYN_ACK;
        seqNo = 1;ackNo = 2;
	                //int ephimeralPort = ntohs(servAddr.sin_port);
                char portno[50];
                //strcpy(port_no,"19327");

                socklen_t servlen = sizeof(servAddr);

                len = sizeof(struct sockaddr_in);
        Getsockname(sockfd, (struct sockaddr *) &servAddr, &len);

                int connSockfd = Socket(AF_INET, SOCK_DGRAM, 0);

                if(isLocal==1)
                {
                        int option = 1;
            Setsockopt(connSockfd, SOL_SOCKET, SO_DONTROUTE, &option, sizeof(option));
                }

                servAddr.sin_port =0;
        Bind(connSockfd, (struct sockaddr *)&servAddr, sizeof(servAddr));

                Getsockname(connSockfd, (struct sockaddr *) &servAddr, &servlen);
                int ephimeralPort = ntohs(servAddr.sin_port);
                printf(" \n ephimeral port no:%d \n ",ephimeralPort);
                sprintf(portno,"%d",ephimeralPort);
                //Signal(SIGALRM, signal_handler);
	       initialize_timer(timer,time_start);


                writePacket(packet,seqNo,ackNo,win,flag,(char *)portno,0);
                Sendto(sockfd, packet, sizeof(pack_struct), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
//              Sendto(sockfd, packet, sizeof(pack_struct), 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));

		if (sigsetjmp(hello_signal,1) != 0){
                        hello_retransmits++;
                        printf("HELLO Timeout occurred");
                        if (hello_retransmits >= RETRANS_MAX){
                                printf("\n Client Down, not sending ACK");
                                return 0;
                        }
                goto send_second_hello;

        }

        //writePacket(packet,seqNo,ackNo,win,flag,(char *)NULL,0);
        //sendPacket(sockfd,packet,HEADER_SIZE,"Second Handshake");


receive_third_hello:
	sigprocmask(SIG_UNBLOCK,&timer_signal_set,NULL);
      len = recvfrom(connSockfd,packet,DATAGRAM_LEN,0,(struct sockaddr *)&cliaddr,&len);
                //recvPacket(sockfd,packet,DATAGRAM_LEN,"Third Hand-Shake");
       readPacket(packet,len,&seqNo,&ackNo,&win,&flag,temp_buffer);
	sigprocmask(SIG_BLOCK,&timer_signal_set,NULL);
       Connect(connSockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr));
	Close(sockfd);
 	initialize_timer(timer,0);
        sigprocmask(SIG_UNBLOCK,&timer_signal_set,NULL);

	initilaize_server_tcb(&sertcb,win,sliding_window_size);
	int filefd = open(file,O_RDONLY);
	if(filefd<0)
                {
                                printf("\nFile Does not exists !!\n");
				sendFin(&sertcb,connSockfd);
                                return 0;
                }
        struct rtt_info info_rtt;
	rtt_init(&info_rtt);
	//initilaize_server_tcb(&sertcb,win,sliding_window_size);
	transmit_data(&connSockfd,filefd,info_rtt,&sertcb);


	//}
	return 1;

	
}

//Listen for connections
void listenForConnections(int *sockfd,struct ifi_info *ifihead,int totalNoIPs) 
{
	int maxFD,i,n,rs;
	struct sockaddr_in cliaddr;
	void  *mesg[512];
	char  filedata[200];
	fd_set readfds;
	char filename[256];
	uint32_t cliWindow = 200;
	tcb *sertcb = (tcb *)malloc(sizeof(tcb));
	int filefd= 0;
	int isLocal;
	
	struct rtt_info info_rtt;
        char *file_name;	
	maxFD=sockfd[totalNoIPs-1]+1;
	FD_ZERO(&readfds);
	for(i=0;i<=totalNoIPs;i++)
	{
		FD_SET(sockfd[i],&readfds);
	}

	signal(SIGCHLD,child_handler);

	while(1) {
	
	while(1)
	{
		
		if (rs=select(maxFD, &readfds, NULL, NULL, NULL))
		if (rs < 0) 
		{       
			//handling case when child exit interrupts parent 
			if (errno == EINTR)
				continue;
			else
				perror("select"); 
		} 
		else if (rs == 0) 
		{
			//waiting for connection
			printf(".");
			fflush(stdout);
		}
		else
			break;
    	}
		
	for(i=0;i<totalNoIPs;i++)
	{
		if(FD_ISSET(sockfd[i], &readfds))
		{
		        struct sockaddr_in servAddr;	
			struct sockaddr_in cliaddr;
            		socklen_t len = sizeof(cliaddr);
			//printf("in isiset ");
			memset(mesg,'\0',512);
			n = recvfrom(sockfd[i],mesg,MAXSIZE,0,(struct sockaddr *)&cliaddr,&len);
			if (n < 0)
                        {
                                //printf("Error in receiving from client!");
				break;
                        }
			pack_struct *packet = (pack_struct *)mesg;
			if (packet->flag != SYN)
				break;
                        socklen_t servlen = sizeof(servAddr);
			printf("\n New connection request  \n");
			pid_t pid;
                        if(checkClient(cliaddr) == 0)
                        {   
				continue;

			}
			//Fork a new process
			if ((pid = Fork()) == 0) {
                                Getsockname(sockfd[i], (struct sockaddr *)&servAddr, &servlen);
				//isLocal = 1;
                                isLocal = is_local(ifihead,&servAddr.sin_addr,&cliaddr.sin_addr);
                                int suc = exchangeHello(sockfd[i],cliaddr,cliWindow,(pack_struct *)mesg,filename,isLocal);				
				exit(0);
			} else {

					addClient(cliaddr,pid);
					latest_child = pid;

				}


		}
	}
   }
	
}
//Get information from server.in
void getPortNWindowInfo()
{
        FILE *fp;
        fp = fopen("server.in","r");
        char portno[200];
        char winsize[200];

        if(fp==NULL)
        {
           printf("Error in opening file");
           return;
        }

        rewind(fp);
        fgets(portno,500,fp);
        printf("port:%s",portno);
        fgets(winsize,200,fp);
        Fclose(fp);

        sscanf(portno,"%d",&server_port_no);
        sscanf(winsize,"%d",&sliding_window_size);
        printf("server_port:%d, winsize:%d",server_port_no,sliding_window_size);

}

int main(int argc, char**argv)
{
	
//Declaring the IFI_HEAD   
   struct ifi_info *ifihead;
   int totalNoIPs,family;
   int *sockfd;
   
 // Get all the required info 
   getPortNWindowInfo();
   family=AF_INET;
   ifihead=Get_ifi_info_plus(family,1);
   totalNoIPs=print_interfaces(ifihead);
   // printf("total ips %d",totalNoIPs);
//Bind discovered Ip address
   sockfd=bindDiscoveredIPs(ifihead,totalNoIPs);

// Start listening to all ports
   listenForConnections(sockfd,ifihead,totalNoIPs);
   
   free_ifi_info_plus(ifihead);
}


