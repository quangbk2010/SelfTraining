#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MYPORT 3490
#define RCVBUFSIZE 32 /* Size of receive buffer */
#define MAXPENDING 5 /* Maximum outstanding connection requests */

int main () {
	int server_sockfd, client_sockfd;

	struct sockaddr_in server_addr;
	unsigned int server_addr_len;
	char *server_message = "Server say Hello!";
	unsigned int len_server_message = strlen (server_message);
	char echoBuffer[RCVBUFSIZE]; /* Buffer for echo string */
	char *server_ip = "192.168.56.101";

	struct sockaddr_in client_addr; /* Client address */
	unsigned int client_addr_len; /* Length of client address data structure */
	char *client_message;
	unsigned int len_client_message;

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
	while (1) {
		client_addr_len = sizeof (client_addr);
		printf("Server: waiting for incoming connections ...\n");
		if ((client_sockfd = accept (server_sockfd, (struct sockaddr *) &client_addr, &client_addr_len)) < 0)
			perror ("accept() failed");

		printf("Server: connections accepted\n");
		
		// Receive message from client
		printf("Server recv ");
		if ((len_client_message = recv (client_sockfd, echoBuffer, RCVBUFSIZE, 0)) < 0)
			perror ("recv() failed");
		if (len_client_message < RCVBUFSIZE)
			echoBuffer[len_client_message] = '\0';
		printf("%s\tlen: %u\n", echoBuffer, len_client_message);

		// send received string and receive again until end of transmission
		while (len_client_message > 0) { // zero indicates end of transmission
			printf("\nServer send " );
			if (send (client_sockfd, server_message, len_server_message, 0) != len_server_message)
				perror ("send() failed");
			//server_message[len_server_message] = '\0';
			printf("%s\n", server_message);

			printf("Server rcv ");
			if ((len_client_message = recv (client_sockfd, echoBuffer, RCVBUFSIZE, 0)) < 0)
				perror ("recv() failed");
			if (len_client_message < RCVBUFSIZE)
				echoBuffer[len_client_message] = '\0';
			printf("%s\tlen: %u\n", echoBuffer, len_client_message);

			sleep (1);
		} 
		//shutdown (server_sockfd, 2);
		close (client_sockfd);
		printf("Client: closed\n");
	}
	return 0;
}
