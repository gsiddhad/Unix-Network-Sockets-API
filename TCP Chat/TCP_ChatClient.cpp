//A TCP Chat Client

#include "Common.h"

int main()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	CheckError(sock, "socket()");

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(9000);

	int result = connect(sock, (sockaddr *)&server_addr, sizeof(server_addr));
	CheckError(result, "connect()");

	TCP_ChatClient(sock);

	close(sock);
	return 0;
}
