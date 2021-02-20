#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8000
#define MAXSZ 1000

int main()
{
	int sockfd;	   //to create socket
	int newsockfd; //to accept connection

	struct sockaddr_in serverAddress; //server receive on this address
	struct sockaddr_in clientAddress; //server sends to client on this address

	int n;
	char msg[MAXSZ];
	int clientAddressLength;
	int pid;
	static int c = 0;
	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	//initialize the socket addresses
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(PORT);

	//bind the socket with the server address and port
	bind(sockfd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

	//listen for connection from client
	listen(sockfd, 5);

	while (1)
	{
		//parent process waiting to accept a new connection
		printf("\n*****server waiting for new client connection:*****\n");
		clientAddressLength = sizeof(clientAddress);
		newsockfd = accept(sockfd, (struct sockaddr *)&clientAddress,
						   &clientAddressLength);
		c = c + 1;
		printf("connected to client: %s\n", inet_ntoa(clientAddress.sin_addr));
		printf("You are client no : %d \n", c);

		//child process is created for serving each new clients
		pid = fork();
		if (pid == 0) //child process rec and send
		{
			//rceive from client
			while (n = recv(newsockfd, msg, MAXSZ, 0) > 0)
			{
				puts("Recieved data from client");
				printf("%s", msg);
				msg[n] = 0;
				//write(newsockfd,msg,n);
				send(newsockfd, msg, n, 0);

				if (n == 0)
				{
					close(newsockfd);
					printf("Goodbye client no : %d \n", c);
					break;
				}

				printf("Received and sent:%s\n", msg);
			} //close interior while
			exit(0);
		}
		else
		{
			close(newsockfd); //sock is closed BY PARENT
		}
	} //close exterior while

	return 0;
}
