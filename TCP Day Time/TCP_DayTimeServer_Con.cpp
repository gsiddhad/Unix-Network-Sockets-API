//TCP DayTime Server (Iterative)

#include"Common.h"

int main() {
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	CheckError(server_sock, "socket()");

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(9000);

	int r = bind(server_sock, (sockaddr*) &server_addr, sizeof(server_addr));
	CheckError(r, "bind()");

	listen(server_sock, 4);

	while (true) {
		int client_sock = accept(server_sock, NULL, NULL);

		int pid = fork();

		if (pid == 0) {
			close(server_sock);
			TCP_DayTimeServer(client_sock);
			close(client_sock);
			break;
		} else if (pid > 0) {
			close(client_sock);
			continue;
		} else {
			cout << "\n Error!!! fork() failed.";
			break;
		}
	}

	close(server_sock);
	return 0;
}
