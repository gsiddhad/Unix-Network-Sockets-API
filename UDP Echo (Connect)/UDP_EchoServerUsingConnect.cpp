//Simple UDP Echo Server using Connect

#include "Common.h"

int main()
{

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	CheckError(sockfd, "socket()");

	sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(9000);

	int r = bind(sockfd, (sockaddr *)&server_addr, sizeof(server_addr));
	CheckError(r, "bind()");

	const int BUFFER_SIZE = 100;
	char Buff[BUFFER_SIZE];

	while (true)
	{

		int count = recvfrom(sockfd, Buff, BUFFER_SIZE, 0,
							 (sockaddr *)&client_addr, &client_addr_len);
		connect(sockfd, (sockaddr *)&client_addr, client_addr_len); //Connect to client

		while (true)
		{
			write(sockfd, Buff, count);

			count = read(sockfd, Buff, count);

			if (Buff[0] == 'Q')
				break;
		}

		client_addr.sin_family = AF_UNSPEC;
		connect(sockfd, (sockaddr *)&client_addr, client_addr_len); //Dis-Connect
	}
	return 0;
}
