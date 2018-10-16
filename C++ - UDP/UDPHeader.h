#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<iostream>
#include<netinet/in.h>
#define MAXLINE 100

using namespace std;

void dgEcho(int sockfd, sockaddr *cliaddr) {
	int n;
	socklen_t len;
	char mesg[MAXLINE];

	while (true) {

		len = sizeof(cliaddr);
		n = recvfrom(sockfd, mesg, MAXLINE, 0, (sockaddr*) &cliaddr, &len);

		if ((mesg[0] == 'Q' || mesg[0] == 'q') && n == 2)
			break;

		sendto(sockfd, mesg, n, 0, (sockaddr*) &cliaddr, len);
	}
}

void dgClient(int server_desc, const sockaddr *server, socklen_t servlen) {
	int n, msglen;
	char sendline[MAXLINE], recvline[MAXLINE + 1];

	cout << "Enter 'Q' or 'q' to quit" << endl;
	while (true) {
		msglen = read(fileno(stdin), sendline, MAXLINE);
		sendto(server_desc, sendline, msglen, 0, server, servlen);

		if (sendline[0] == 'Q' || sendline[0] == 'q')
			break;

		n = recvfrom(server_desc, recvline, MAXLINE, 0, NULL, NULL);

		recvline[n] = 0;
		write(fileno(stdout), recvline, n);
	}
}
