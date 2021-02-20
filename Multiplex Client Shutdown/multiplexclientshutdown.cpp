//Multiplex Client using shutdown()

#include "Common.h"

void MULTIPLEX_SHUTDOWN_Client(FILE *fp, int sockfd)
{
	bool write_closed = false;
	int maxfdp1;
	fd_set rset;
	char sendline[100], recvline[100];

	FD_ZERO(&rset);

	while (true)
	{
		if (write_closed == false)	   //Check if write-half of connection has been closed
			FD_SET(fileno(fp), &rset); //Enable fileno(fp) in rset

		FD_SET(sockfd, &rset); //Enable sockfd in rset

		maxfdp1 = (fileno(fp) > sockfd ? fileno(fp) : sockfd) + 1; //Max File Desc Plus 1

		select(maxfdp1, &rset, NULL, NULL, NULL); //Wait untill any fd in rset is ready for reading

		if (FD_ISSET(sockfd, &rset))
		{												  //sockfd is ready for reading
			int read_count = read(sockfd, recvline, 100); //Read from sockfd

			if (read_count == 0)
			{ //EOF recieved
				if (write_closed)
					cout << "\n Connection Closed! \n\n";
				else
					cout << "\n Server Aborted Abnormaly!!!";
				break;
			}
			cout << "\n Msg From Server : ";
			fflush(stdout);
			write(fileno(stdout), recvline, read_count); //Write responce from server to stdout
		}
		if (FD_ISSET(fileno(fp), &rset))
		{													  //fp is ready for reading
			int read_count = read(fileno(fp), sendline, 100); //Read from fp
			if (read_count == 0)
			{						 //EOF recieved
				write_closed = true; //Shutdown write-half of sockfd
				shutdown(sockfd, SHUT_WR);
				continue;
			}
			write(sockfd, sendline, read_count); //write data to sockfd
		}
	}
}

int main()
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(9000);

	connect(sock, (sockaddr *)&server_addr, sizeof(server_addr));

	MULTIPLEX_SHUTDOWN_Client(stdin, sock);

	close(sock);
	return 0;
}
