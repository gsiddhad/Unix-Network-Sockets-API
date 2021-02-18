// sigpipe-server.c

/*** Demonstrates how to generate a SIGPIPE error with a socket.           ***/
/*****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <stdlib.h>

void sig_pipe(int signum) {
	printf("got it!\n");

}

/*---------------------------------------------------------------------*/
/*--- main - allow client to connect and then terminate.            ---*/
/*---------------------------------------------------------------------*/
main() {
	struct sockaddr_in addr;
	int sd, byte_count, addr_size;

	signal(SIGPIPE, sig_pipe);
	if ((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket");
		abort();
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(9988);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sd, (struct sockaddr*) &addr, sizeof(addr)) != 0) {
		perror("Bind");
		abort();
	}
	if (listen(sd, 20) != 0) {
		perror("Listen");
		abort();
	}
	while (1) {
		int client;
		char buffer[1024];
		addr_size = sizeof(addr);
		client = accept(sd, (struct sockaddr*) &addr, &addr_size);
		printf("Connected: %s:%d\n", inet_ntoa(addr.sin_addr),
				ntohs(addr.sin_port));
		recv(client, buffer, 4, 0);
		close(client);
	}
	close(sd);
}

