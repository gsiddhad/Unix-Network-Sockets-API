//A Simple Client 

#include"Common.h"

//SIGPIPE Handler 
void SIGPIPE_Handler(int sig) {
	cout << "\n\n SIGPIPE Intercepted ";
	fflush (stdout);
}

int main() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(9000);

	connect(sock, (sockaddr*) &server_addr, sizeof(server_addr));

	signal(SIGPIPE, SIGPIPE_Handler);

	char msg[40];
	int len;
	gets(msg);

	len = write(sock, msg, strlen(msg));
	CheckError(len, "write()", false);

	len = write(sock, msg, strlen(msg));
	CheckError(len, "write()", false);

	close(sock);
	return 0;
}
