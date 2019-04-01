#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define RCVBUFSIZE 32 /* Size of receive buffer*/
#define SERVPORT 3490

int main () {

	int sock;
	struct sockaddr_in server_addr, client_addr;
	char *server_ip = "127.0.0.1";
	unsigned int len_client_addr, len_server_addr;
	char client_message[20] = "Client say hello";
	
	char echoBuffer[RCVBUFSIZE];        /*Buffer for echo string*/
	unsigned int len_client_message;    /*Length of string to echo*/
	unsigned int len_server_message;
	
	/*Create a reliable, stream socket using TCP*/
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		perror ("socket() failed");
	
	/*Construct the server address structure*/
	server_addr.sin_family = AF_INET; /*Internet address family*/
	server_addr.sin_addr.s_addr = inet_addr (server_ip);
	//server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // use IP address of physical machine
	server_addr.sin_port = htons (SERVPORT);	
	memset (&(server_addr.sin_zero), 0, 8);
	
	len_client_message = strlen (client_message);
	int i = 0;
	char s[3];
	printf("Client start send and recv loop\n");
	while (i < 10) {
		len_client_addr = sizeof (client_addr);
		len_server_addr = sizeof (server_addr);
		/*Send the string to the server*/
		printf("Client send: %s\tLen: %u\n", client_message, len_client_message);
		if (sendto (sock, client_message, len_client_message, 0, (struct sockaddr *) &server_addr, sizeof (server_addr)) != len_client_message)
			perror ("sendto() sent a different number of bytes than expected");

		i ++;
		sprintf (s, "%d", i);
		strcpy (client_message, s);
		len_client_message = strlen (client_message);
		
		/*Receiver string from the server*/
		printf("Client recv: ");
		/*if (len_server_message = recvfrom (sock, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *) &client_addr, &len_client_addr) < 0)
			perror ("recvfrom() failed");
		//if (len_server_message < RCVBUFSIZE)
			//echoBuffer[len_server_message] = '\0';
		printf ("%s\tLen: %u\n", echoBuffer, len_server_message);*/
		if ((len_server_message = recvfrom (sock, echoBuffer, RCVBUFSIZE, 0, (struct sockaddr *)&server_addr, &len_server_addr))< 0)
			perror ("recvfrom() failed");
		if (len_server_message < RCVBUFSIZE)
			echoBuffer[len_server_message] = '\0';
		printf("Server recv: %s\tLen: %u\n", echoBuffer, len_server_message);

		
	}
	//shutdown (sock, 2);
	close (sock);
	printf("Client: closed\n");
	return (0);
}
