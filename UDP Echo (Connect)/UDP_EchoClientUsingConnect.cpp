//UDP Client which uses connect()

#include "Common.h"

int main()
{

	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	CheckError(sockfd, "socket()");

	sockaddr_in server_addr;
	socklen_t server_addr_len = sizeof(server_addr);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(9000);

	const int BUFFER_SIZE = 100;
	char Buff[BUFFER_SIZE];

	connect(sockfd, (sockaddr *)&server_addr, server_addr_len);
	while (true)
	{
		int count = read(fileno(stdin), Buff, BUFFER_SIZE);

		count = write(sockfd, Buff, count);
		CheckError(count, "write()");

		count = read(sockfd, Buff, BUFFER_SIZE);
		CheckError(count, "write()");

		write(fileno(stdout), Buff, count);
	}
	return 0;
}
