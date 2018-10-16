#include<iostream>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>

using namespace std;

void SIGCHLD_Handler(int sig) {
	//fputs("\nSIGCHLD Intercepted",stdout);
	cout << "\nSIGCHLD Intercepted";
	fflush(stdout);
	int status;

	while (waitpid(-1, &status, WNOHANG) > 0)
		;

} //Handler

int main() {

	int c = 0;
	int server_desc = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(9000);

	bind(server_desc, (sockaddr*) &server_addr, sizeof(server_addr));

	listen(server_desc, 4);

	bool Runserver = true;

	signal(SIGCHLD, SIGCHLD_Handler);

	while (Runserver) {
		int client_desc = accept(server_desc, NULL, NULL);

		int pid = fork();

		if (pid > 0) {
			close(client_desc);
			continue;
		} else if (pid == 0) {
			close(server_desc);
			char msg[4096];
			int msg_len = 0;

			while ((msg_len = read(client_desc, msg, 4096)) > 0) {
				write(client_desc, msg, msg_len);
			} //while
			Runserver = false;
		} //else if
		close(client_desc);
	} //while

} //main
