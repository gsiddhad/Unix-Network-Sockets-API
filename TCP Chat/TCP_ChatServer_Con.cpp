// TCP Chat Server (Concurrent)

#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h> //For sockaddr_in
#include <unistd.h>
using namespace std;

#include "Common.h"

int main()
{
	//Create Socket
	int server_desc = socket(AF_INET,	  //Protocol Family (AF_INET, AF_INET6, AF_LOCAL, ...) (Can Use PF_xxx constants)
							 SOCK_STREAM, //Socket Type (SOCK_STREAM, SOCK_DGRAM, SOCK_ROW, SOCK_SEQPACKET)
							 0			  //Protocol (IPPROTO_TCP, IPPROTO_UDP, IPPROTO_SCTP, 0 -> Auto Select)
	);
	CheckError(server_desc, "socket()");

	//Create and Fill Address Structure for this Server
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;		  //Address Family (AF_INET, AF_INET6, AF_LOCAL, ...)
	server_addr.sin_addr.s_addr = INADDR_ANY; //Internet Address (INADDR_ANY-> Accept connection at any IP Address)
	server_addr.sin_port = htons(9000);		  //Port Number (htons -> h.HOST t.TO n.NETWORK s.SHORT , Ensures proper byte ordering)

	//Bind Socket Descriptor and Address Structure together
	int result = bind(server_desc,				//Server Descriptor
					  (sockaddr *)&server_addr, //Address Structure
					  sizeof(server_addr)		//Address Structure Length
	);
	CheckError(result, "bind()");

	//Start Listioning (Tell kernel to accept connections directed towards this socket) (Puts socket into passive mode)
	listen(server_desc, //Server Descriptor
		   4			//Backlog (Different implementations have different meaning)
	);

	//Server Loop
	bool RunServer = true;
	while (RunServer)
	{
		//Accept a Connection (Puts process in sleep mode if Connection Queue is Empty)
		int client_desc;
		client_desc = accept(server_desc, //Listening Socket
							 NULL,		  //Pointer to Address Structure for Client (Filled By Kernel) (Set NULL if not needed)
							 NULL		  //Pointer to Size of Address Structure (FIlled By Kernel) (Set NULL if not needed)
		);
		CheckError(client_desc, "accept()");

		//Create Child Process to handle connection
		int pid = fork();

		if (pid > 0) //Parent Process
		{
			//Close Client Socket
			close(client_desc);
			continue;
		}
		else if (pid == 0) //Child Process
		{
			//Close Listening Socket
			close(server_desc);

			TCP_ChatServer(client_desc);

			//Close Connection
			close(client_desc);
			break; //Work Done! Exit Child Process.
		}
		else
		{
			cout << "fork() Error!!!";
			break;
		}
	}

	return 0;
}
