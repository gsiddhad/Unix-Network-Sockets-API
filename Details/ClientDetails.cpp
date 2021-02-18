#include<iostream>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>

using namespace std;

int main() {

	int c = 0;
	int server_desc = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_addr, client_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(9000);

	bind(server_desc, (sockaddr*) &server_addr, sizeof(server_addr));

	listen(server_desc, 4);
	socklen_t client_addr_len = sizeof(client_addr);

	int client_desc = accept(server_desc, (sockaddr*) &client_addr,
			&client_addr_len);
	char buff[INET_ADDRSTRLEN];

	cout << "Client Details\n";
	cout << "IP Addr : ";
	inet_ntop(AF_INET, (void*) &client_addr.sin_addr, buff, sizeof(buff));

	cout << buff;

	cout << "\nPort : " << ntohs(client_addr.sin_port);

	close(client_desc);

	return 0;
} //main
