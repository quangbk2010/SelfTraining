#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/types.h>  
#include <sys/socket.h> /* for socket(), connect(), sendto(), and recvfrom() */
#include <netinet/in.h> /* for sockaddr_in, sockaddr */ /* we can use another library: arpa/inet.h */
#include <string.h>     /* for memset(), strlen, strcpy, strcat,... */
#include <stdlib.h>     /* for atoi() and exit() */
#include <unistd.h>     /* for close() */
#include <arpa/inet.h>

#define RCVBUFSIZE 32 /* Size of receive buffer*/
#define SERVPORT 3490

int main () {

	int sock;
	struct sockaddr_in server_addr;
	char *server_ip = "192.168.56.101"; /* local interface*/
	char client_message[20] = "Client say hello";
	
	char echoBuffer[RCVBUFSIZE];        /*Buffer for echo string*/
	unsigned int len_client_message;    /*Length of string to echo*/
	int bytesRcvd, totalBytesRcvd;      /*Bytes read in single recv()
									       and total bytes read*/
	puts ("TCP client start.");
	/*Create a reliable, stream socket using TCP*/
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		perror ("socket() failed");
	puts ("socket success");
	/*Construct the server address structure*/
	server_addr.sin_family = AF_INET; /*Internet address family*/
	server_addr.sin_addr.s_addr = inet_addr (server_ip);
	//server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // use IP address of physical machine
	server_addr.sin_port = htons (SERVPORT);	
	memset (&(server_addr.sin_zero), 0, 8);
	
	/*Establish the connection to the echo server*/
	if (connect(sock, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0)
		perror ("connect() failed");
	len_client_message = strlen (client_message);
	int i = 0;
	char s[3];
	printf(" Start send and recv loop\n");
	while (i < 10) {
		/*Send the string to the server*/
		printf("Client send %s\n", client_message);
		if (send (sock, client_message, len_client_message, 0) != len_client_message)
			perror ("send() sent a different number of bytes than expected");
		
		i ++;
		sprintf (s, "%d", i);
		strcpy (client_message, s);
		len_client_message = strlen (client_message);
		//strcat (client_message, s);

		/*Receiver string from the server*/
		totalBytesRcvd = 0;
		printf ("Client received: "); 
		while (totalBytesRcvd < len_client_message) {
			/*Receive up to the buffer size from the sender*/
			if ((bytesRcvd = recv (sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
				perror ("recv() failed or connection closed prematurely");
				
			totalBytesRcvd += bytesRcvd; /*keep totally of total bytes*/
			if (bytesRcvd < RCVBUFSIZE)
				echoBuffer[bytesRcvd] = '\0'; /*terminate the string*/
			printf ("%s", echoBuffer);
		}
		printf ("\n");
		
	}
	//shutdown (sock, 2);
	close (sock);
	printf("Client: closed\n");
	return (0);
}
