//UDP DayTime Server

#include"Common.h"

int main() {
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	CheckError(sock, "socket()");

	sockaddr_in server_addr, client_addr;
	socklen_t client_addr_len = sizeof(server_addr);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(9000);

	int r = bind(sock, (sockaddr*) &server_addr, sizeof(server_addr));
	CheckError(r, "bind()");

	while (true) {
		char Buff[1];
		recvfrom(sock, Buff, 1, 0, (sockaddr*) &client_addr, &client_addr_len);
		UDP_DayTimeServer(sock, client_addr, client_addr_len);
	}
	return 0;
}

