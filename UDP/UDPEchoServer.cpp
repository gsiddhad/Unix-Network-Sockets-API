#include "UDPHeader.h"

int main(int args, char *argv[])
{
	int serv_desc = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in serv_addr, client;

	serv_desc = socket(AF_INET, SOCK_DGRAM, 0);
	if (serv_desc == -1)
	{
		puts("socket can not be created\n");
		return -1;
	}

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(9000);

	if (bind(serv_desc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		puts("bind failed\n");
		return -1;
	}
	cout << "server started" << endl;

	dgEcho(serv_desc, (sockaddr *)&client);

	cout << "exited" << endl;
	return 0;
}
