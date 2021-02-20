#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main()
{

	int c = 0;
	int server_desc = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(9000);

	bind(server_desc, (sockaddr *)&server_addr, sizeof(server_addr));

	listen(server_desc, 4);

	bool Runserver = true;

	int client_cout = 0;
	while (Runserver)
	{
		int client_desc = accept(server_desc, NULL, NULL);
		cout << c++ << endl;
		int pid = fork();

		if (pid > 0)
			continue;
		else if (pid == 0)
		{
			char msg[4096];
			int msg_len = 0;

			while ((msg_len = read(client_desc, msg, 4096)) > 0)
			{
				if (msg[0] == 'Q' && msg[1] == 'T')
				{
					Runserver = false;
					break;
				} //if
				else
					write(client_desc, msg, msg_len);
			} //while
		}	  //else if

	} //while

} //main
