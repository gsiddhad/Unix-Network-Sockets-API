// Client Showing IO Multiplexing
//A Simple Client
#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>

using namespace std;

void MULTIPLEX_Client(FILE *fp, int sockfd) {
	int maxfdp1;
	fd_set rset;
	char sendline[100], recvline[100];

	FD_ZERO(&rset);

	while (true) {
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);

		maxfdp1 = ( fileno(fp) > sockfd ? fileno(fp) : sockfd) + 1;

		select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {
			int read_count = read(sockfd, recvline, 100);
			if (read_count == 0) {
				cout << "\n Server Aborted! \n\n";
				break;
			}
			cout << "\n Msg From Server : ";
			fflush(stdout);
			write(fileno(stdout), recvline, read_count);
		}
		if (FD_ISSET(fileno(fp), &rset)) {
			int read_count = read(fileno(fp), sendline, 100);
			if (read_count == 0)
				break;
			write(sockfd, sendline, read_count);
		}
	}
}

int main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(9000);

	connect(sock, (sockaddr*) &server_addr, sizeof(server_addr));

	MULTIPLEX_Client(stdin, sock);

	close(sock);
	return 0;
}
