#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 1024

int main(int argc, char **argv)
{
	int client_len;
	int client_sockfd;
		
	char buf_in[MAXLINE];
	char buf_get[MAXLINE];
	
	struct sockaddr_un clientaddr;
	
	if (argc != 2)
	{
		printf("Usage : %s [file_name]\n", argv[0]);
		printf("example : %s /tmp/mysocket\n", argv[0]);
		exit(0);
	}
	
	client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (client_sockfd == -1)
	{
		perror("error : ");
		exit(0);
	}
	bzero(&clientaddr, sizeof(clientaddr));
	clientaddr.sun_family = AF_UNIX;
	strcpy(clientaddr.sun_path, argv[1]);
	client_len = sizeof(clientaddr);

	if (connect(client_sockfd, (struct sockaddr *)&clientaddr, client_len) < 0)
	{
		perror("Connect error: ");
		exit(0);
	}
	while(1)
	{
		memset(buf_in, 0x00, MAXLINE);
		memset(buf_get, 0x00, MAXLINE);
		printf("> ");
		fgets(buf_in, MAXLINE, stdin);
		write(client_sockfd, buf_in, strlen(buf_in));
		read(client_sockfd, buf_get, MAXLINE);
		printf("From server -> %s", buf_get);
	}

	close(client_sockfd);
	exit(0);
}
