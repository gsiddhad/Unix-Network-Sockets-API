//TCP DayTime Client

#include "Common.h"

int main()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	CheckError(sock, "socket()");

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(9000);

	int r = connect(sock, (sockaddr *)&server_addr, sizeof(server_addr));
	CheckError(r, "connect()");

	char Buff[200];
	int Count = read(sock, Buff, 200);

	if (Count > 0)
	{
		cout << "\n Current Date & Time (Local) is : ";
		fflush(stdout);
		write(fileno(stdout), Buff, Count);
		cout << "\n\n";
	}
	else
		cout << "Server Terminated without Responce";

	close(sock);
	return 0;
}
