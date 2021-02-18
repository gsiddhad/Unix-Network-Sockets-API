#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>

const int ASSIGNED_PORT = 17000;
const int BUF_SIZE = 512;

int main() {

	int serverfd, clientfd;
	struct sockaddr_in serverSock; //NOTE: a pointer to sockaddr_in can be cast to a pointer to 
	//      a struct sockaddr - useful for connect()

	char buf[BUF_SIZE];
	int errorCheck, msgLength;

	//create socket with error checking (-1 ret on error)
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (serverfd < 0) {
		perror("socket failed.");
		exit(errno);
	}

	//assign sockaddr_in info for server
	bzero(&serverSock, sizeof(serverSock));     //set to all 0's
	serverSock.sin_family = AF_INET;
	serverSock.sin_addr.s_addr = htonl(INADDR_ANY);
	serverSock.sin_port = htons(ASSIGNED_PORT);

	//bind a name to the socket with error checking (0 ret on success)
	errorCheck = bind(serverfd, (struct sockaddr*) &serverSock,
			sizeof(serverSock));
	if (errorCheck < 0) {
		perror("bind failed.");
		exit(errno);
	}

	//listen for connections with error checking (0 ret on success)
	errorCheck = listen(serverfd, 10);
	if (errorCheck < 0) {
		perror("listen failed.");
		exit(errno);
	}

	printf("Listening for connections.  Enter CNTRL-c to kill server.\n");

	//create infinite loop to accept, read, write, and close connections with error hecking 
	while (1) {

		//accept the connection from the client 
		clientfd = accept(serverfd, 0, 0);
		if (clientfd == -1) {
			perror("error accepting connection.");
			exit(errno);
		}

		//read data from the client
		bzero(buf, BUF_SIZE);
		msgLength = read(clientfd, buf, BUF_SIZE - 1);
		if (msgLength == -1) {
			perror("error reading from client");
			close(clientfd);
			close(serverfd);
			exit(errno);
		}

		if (buf[0] == '\0') {
			printf("connection closing");
			exit(0);
		}
		//print what the client sent
		printf("Message from client: %s\n", buf);

		//echo back what the client sent
		errorCheck = write(clientfd, buf, strlen(buf) + 1);

		if (errorCheck == -1) {
			perror("error writing to client");
			exit(errno);
		}

		//close the connection
		errorCheck = close(clientfd);
		if (errorCheck == -1) {
			perror("error closing connection");
			exit(errno);
		}
	}

	errorCheck = close(serverfd);
	if (errorCheck == -1) {
		perror("error closing server, exiting program now");
		sleep(6);
		exit(errno);
	}
}
