#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <resolv.h>
#include <arpa/inet.h>

const int BUF_SIZE = 512;

int main(int argc, char *argv[]) {

	char buf[BUF_SIZE], buf2[BUF_SIZE];
	char *msg;
	struct sockaddr_in serverInfo;
	int clientSockfd, errorCheck, readVal, numfd;
	struct hostent *hostName;
	fd_set readfds;

	//make sure user entered correct arguments when starting client
	if (argc != 3) {
		printf("error: must enter 'programName portNumber hostname'\n");
		exit(errno);
	}

	//create socket and error check socket() call
	clientSockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSockfd == -1) {
		perror("error creating socket");
		exit(errno);
	}

	//assign sockaddr_in info for RemoteAddr
	bzero(&serverInfo, sizeof(serverInfo));
	serverInfo.sin_family = AF_INET;

	hostName = gethostbyname(argv[2]);
	if (hostName == NULL) {
		herror("Error when calling gethostbyname()");
		exit(errno);
	}

	memcpy((unsigned char *) &serverInfo.sin_addr,
			(unsigned char *) hostName->h_addr, hostName->h_length); //copy IP address to be used
	serverInfo.sin_port = htons(atoi(argv[1])); //port number to be used, given in command line, must be converted to network byte order

	//connect to server side
	if (connect(clientSockfd, (struct sockaddr *) &serverInfo,
			sizeof(serverInfo)) == -1) {
		perror("error when connecting to server");
		exit(errno);
	}

	while (1) {
		FD_ZERO(&readfds);  //zero out set
		FD_SET(fileno(stdin), &readfds);
		FD_SET(clientSockfd, &readfds);

		int maxfd = fileno(stdin);
		if (maxfd < clientSockfd)
			maxfd = clientSockfd;

		numfd = select(maxfd + 1, &readfds, 0, 0, 0);

		if (numfd > 0) {
			if (FD_ISSET(clientSockfd, &readfds)) {
				//make sure buf is empty so it doesnt print extra chars
				bzero(buf2, BUF_SIZE);
				read(clientSockfd, buf2, BUF_SIZE);

			}
			if (FD_ISSET(fileno(stdin), &readfds)) {
				bzero(buf, BUF_SIZE);
				fgets(buf, BUF_SIZE - 1, stdin);
				printf("echo from server: %s\n", buf);
				errorCheck = write(clientSockfd, buf, strlen(buf) + 1);
				if (errorCheck == -1) {
					perror("error writing");
				}
			}
		} else if (numfd == 0) {
			perror("Error using select()\n");
			exit(0);
		} else
			printf("no data\n");
	}
	//close connection to server
	errorCheck = close(clientSockfd);
	if (errorCheck == -1) {
		perror("Error closing connection.");
		exit(errno);
	}
	return 0;
}
