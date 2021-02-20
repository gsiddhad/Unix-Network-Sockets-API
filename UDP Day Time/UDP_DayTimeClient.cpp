//UDP DayTime Client

#include "Common.h"

int main()
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	CheckError(sock, "socket()");

	sockaddr_in server_addr;
	socklen_t addr_len = sizeof(server_addr);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(9000);

	char Buff[200];

	sendto(sock, Buff, 1, 0, (sockaddr *)&server_addr, addr_len);
	int Count = recvfrom(sock, Buff, 200, 0, (sockaddr *)&server_addr,
						 &addr_len);
	if (Count > 0)
	{
		cout << "\n Current Date & Time (Local) is : ";
		fflush(stdout);
		write(fileno(stdout), Buff, Count);
		cout << "\n\n";
	}
	else
		cout << "Empty Responce from Server";

	return 0;
}
