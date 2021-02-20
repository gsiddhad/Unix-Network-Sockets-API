//TCP FileServer (Iterative)

#include "Common.h"

void SIGPIPE_Handler(int sig)
{
	cout << "\n Client Terminated Prematurley";
	exit(1);
}

int main()
{
	int server_sock = socket(AF_INET, SOCK_STREAM, 0);
	CheckError(server_sock, "socket()");

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(9000);

	int r = bind(server_sock, (sockaddr *)&server_addr, sizeof(server_addr));
	CheckError(r, "bind()");

	listen(server_sock, 4);

	signal(SIGPIPE, SIGPIPE_Handler);

	while (true)
	{
		int client_sock = accept(server_sock, NULL, NULL);

		int pid = fork();

		if (pid == 0)
		{
			close(server_sock);
			TCP_FileServer(client_sock);
			close(client_sock);
			break;
		}
		else if (pid > 0)
		{
			close(client_sock);
			continue;
		}
		else
		{
			cout << "\n Error!!! fork() failed.";
			close(server_sock);
			break;
		}
	}

	return 0;
}
