#include "UDPHeader.h"

using namespace std;

int main() {
	int serv_desc = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in serv_addr;

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(9000);

	cout << "client started" << endl;

	dgClient(serv_desc, (sockaddr*) &serv_addr, sizeof(serv_addr));

	cout << "exited" << endl;
	return 0;
}
