#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
int main() {
	struct sockaddr_in server, client;
	socklen_t len;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(8100);
	len = sizeof(server);
	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0) {
		printf("error in creating socket\n");
		return -1;
	}
	if (bind(socketfd, (struct sockaddr*) &server, len) < 0) {
		printf("bind failed \n");
	}
	listen(socketfd, 4);
	for (;;) {
		int clientfd = accept(socketfd, (struct sockaddr *) &client, &len),
				readLen;
		char msg[1000];
		if (fork() != 0) {
			int pid = getpid();
			printf("Child PID : %d\n", pid);
			close(socketfd);
			while ((readLen = recv(clientfd, msg, 1000, 0)) > 0) {
				printf("recieved\n");
				write(clientfd, msg, readLen);
			}
			if (readLen == 0) {
				printf("Client disconnected!!\n");
				return 0;
			}
		} else {
			struct sockaddr_in peer;
			char ip[20];
			close(clientfd);
			socklen_t peerLen = sizeof(peer);
			int ret = getpeername(clientfd, (struct sockaddr*) &peer, &peerLen);
			if (ret != -1) {
				printf(
						"Client is listening at port no. : %d\nIP address : %s\n",
						ntohs(peer.sin_port),
						inet_ntop(AF_INET, (struct addr*) &peer.sin_addr, ip,
								sizeof(ip)));
				printf("ip : %s\n", ip);
			}
			printf("Listening....");
		}
	}
	return 0;
}
