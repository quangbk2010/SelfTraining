#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define MYPORT 3490
#define RCVBUFSIZE 32 /* Size of receive buffer */
#define MAXPENDING 5 /* Maximum outstanding connection requests */

int main () {
	int server_sockfd;

	struct sockaddr_in server_addr;
	unsigned int server_addr_len;
	char server_message[20] = "Server say Hello!";
	unsigned int len_server_message = strlen (server_message);
	char echoBuffer[RCVBUFSIZE]; /* Buffer for echo string */
	char *server_ip = "127.0.0.1";

	struct sockaddr_in client_addr; /* Client address */
	unsigned int client_addr_len; /* Length of client address data structure */
	char *client_message;
	unsigned int len_client_message;

	if ((server_sockfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
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

	// Repeatedly:
	//a. Accept new connection
	//b. Communicate
	//c. Close the connection

	printf("Server start send and recv loop\n");
	while (1) {
		client_addr_len = sizeof (client_addr);
		/*Block until receive message from a client*/
		//printf("Server recv\n");
		if ((len_client_message = recvfrom (server_sockfd, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len))< 0)
			perror ("recvfrom() failed");
		if (len_client_message < RCVBUFSIZE)
			echoBuffer[len_client_message] = '\0';
		printf("Server recv: %s\tLen: %u\n", echoBuffer, len_client_message);

		/*Send messages to the client*/
		sprintf (server_message, "%.1f", atoi (server_message) + 1.5);
		len_server_message = strlen (server_message);
		if (sendto (server_sockfd, server_message, len_server_message, 0, (struct sockaddr *) &client_addr, sizeof (client_addr)) != len_server_message)
			perror ("sendto() sent a different number of bytes than expected");
		printf("Server send: %s\tLen: %u\n", server_message, len_server_message);

		sleep (1);
	}		
	return 0;
}
