#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h> // for inet_ntoa function

#define MYPORT 3490
#define RCVBUFSIZE 32 /* Size of receive buffer */
#define MAXPENDING 5 /* Maximum outstanding connection requests */

void HandleTCPClient(void *client_sockfd);
typedef struct client
{
	int sockfd;
	char *ip;
} Client;

int main () {
	int server_sockfd, client_sockfd, *new_sockfd;

	struct sockaddr_in server_addr;
	unsigned int server_addr_len;
	
	char *server_ip = "127.0.0.1", *client_ip;
	Client *client;

	struct sockaddr_in client_addr; /* Client address */
	unsigned int client_addr_len; /* Length of client address data structure */
	char *client_message;
	unsigned int len_client_message;
	//char *some_addr;
	int some_addr;
	int pid;

	if ((server_sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
		perror ("socket() failed");
	printf("Server: create socket.\n");

	server_addr.sin_family = AF_INET;       // host by order
	server_addr.sin_port = htons (MYPORT);  // short, network byte order
	server_addr.sin_addr.s_addr = inet_addr(server_ip); // covert to 32bit addr
	//server_addr.sin_port = htons(0); // choose an unused port at random
    //server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // use IP address of physical machine
	memset (&(server_addr.sin_zero), '\0', 8); // sero the rest of the struct

	// assign a port to socket
	// error checking for bind()
	if (bind (server_sockfd, (struct sockaddr *) &server_addr, sizeof (struct sockaddr)) < 0)// try to use automatic cast? just get a compiler warning?: bind (sockfd, server_addr, sizeof (struct sockaddr));
		perror ("bind() failed");
	printf("Server: bind done\n");

	// Mark the socket so it will listen for incoming connections
	if (listen (server_sockfd, MAXPENDING) < 0)
		perror ("listen() failed");
	printf("Server: listen done\n");


	// Repeatedly:
	//a. Accept new connection
	//b. Communicate
	//c. Close the connection
	int i = 0;
	client_addr_len = sizeof (client_addr);
	printf("Server: waiting for incoming connections ...\n");
	int n;
		
	while (client_sockfd = accept (server_sockfd, (struct sockaddr *) &client_addr, &client_addr_len) ) {
		
		printf("Server: connections accepted, client_sockfd = %d\n", client_sockfd);	
		client_ip = inet_ntoa (client_addr.sin_addr);
		pthread_t thread;
		printf("a\n");
		new_sockfd = malloc (1);
		*new_sockfd = client_sockfd;
		printf("b\n");
		client = (Client *) malloc (sizeof (Client));
		client -> sockfd = client_sockfd;
		printf("c\n");
		client -> ip = client_ip;
		printf("d\n");



		/*if (pthread_create (&thread, NULL, HandleTCPClient, (void *) client) < 0) {
		//if (pthread_create (&thread, NULL, HandleTCPClient, (void *) new_sockfd) < 0) {
			perror ("could not create thread");
			exit (1);
		}
		pthread_join (thread, NULL);*/

		pid=fork();
		if(pid==0) {//child process rec and send 
			//rceive from client
			/*while(1) {
				n=recv(new_sockfd,msg,MAXSZ,0);
				if(n==0) {
		   	    	close(new_sockfd);
				    break;
				}
				msg[n]=0;
				send(new_sockfd,msg,n,0);

				printf("Receive and set:%s\n",msg);
			}//close interior while*/
			HandleTCPClient ((void *) new_sockfd);
			exit(0);
		}
		else
		    close(new_sockfd);//sock is closed BY PARENT
				
	}
	//shutdown (server_sockfd, 2);
	close (server_sockfd);
	printf("Server: closed\n");
	return 0;
}

// handle received request
void HandleTCPClient(void *clnt) {

	printf("AAAA\n");
	Client client = *(Client *) clnt;
	int client_sockfd = client.sockfd;
	char *ip = client.ip;

	//int client_sockfd = *(int *) clnt;

	char echoBuffer[RCVBUFSIZE];  /* Buffer for echo string */
	int len_client_message;       /* Size of message received from server */
	char *server_message = "Server say Hello!";
	unsigned int len_server_message = strlen (server_message);

	// Receive message from client
	printf("Server recv from %s ", ip);
	if ((len_client_message = recv (client_sockfd, echoBuffer, RCVBUFSIZE, 0)) < 0)
		perror ("recv() failed");
	if (len_client_message < RCVBUFSIZE)
		echoBuffer[len_client_message] = '\0';
	printf("%s\tlen: %u\n", echoBuffer, len_client_message);
	
	// send received string and receive again until end of transmission
	while (len_client_message > 0) { // zero indicates end of transmission
		printf("\nServer send to %s ", ip );
		if (send (client_sockfd, server_message, len_server_message, 0) != len_server_message)
			perror ("send() failed");

		//server_message[len_server_message] = '\0';
		printf("%s\n", server_message);
		printf("Server rcv from %s ", ip);
		if ((len_client_message = recv (client_sockfd, echoBuffer, RCVBUFSIZE, 0)) < 0)
			perror ("recv() failed");
		if (len_client_message < RCVBUFSIZE)
			echoBuffer[len_client_message] = '\0';
		printf("%s\tlen: %u\n", echoBuffer, len_client_message);
		sleep (1);
	} 

	// Free the socket pointer
	free (clnt);
}