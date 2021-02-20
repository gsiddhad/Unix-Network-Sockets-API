//TCP File Client

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

	const int BUFFER_SIZE = 4096;
	char FileName[BUFFER_SIZE];

	//Get file name
	cout << "\n Enter file name:";
	cin >> FileName;

	TCP_FileClient(sock, FileName);

	close(sock);
	return 0;
}
