// Singnal Handling:  A server which close connection  immediately affter accepting it

#include "Common.h"

int main()
{

	int server_desc = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(9000);

	bind(server_desc, (sockaddr *)&server_addr, sizeof(server_addr));

	listen(server_desc, 4);

	while (true)
	{
		int client_desc = accept(server_desc, NULL, NULL);
		close(client_desc);
	}

	close(server_desc);
	return 0;
}
