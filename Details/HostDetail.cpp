#include <netdb.h>
#include <iostream>
#include <arpa/inet.h>

using namespace std;

void GetHostByName();
void GetHostByAddr();

int main()
{
	cout << "gethostbyname() :" << endl;
	GetHostByName();

	cout << "gethostbyaddr() :" << endl;
	GetHostByAddr();

	return 0;
}

void GetHostByName()
{
	struct hostent *host;

	host = gethostbyname("google.co.in");

	if (host != NULL)
	{
		cout << "Host name  :" << host->h_name << endl;
		cout << "Aliases  :";

		for (int i = 0; host->h_aliases[i] != NULL; i++)
			cout << "\t" << host->h_aliases[i];

		cout << endl;

		cout << "Address length  :" << host->h_length << endl;

		char str[INET_ADDRSTRLEN];

		cout << "IP Address list  :";
		for (int i = 0; host->h_addr_list[i] != NULL; i++)
			if (host->h_addrtype == AF_INET)
				cout << "\t"
					 << inet_ntop(AF_INET, host->h_addr_list[i], str,
								  INET_ADDRSTRLEN);

		cout << endl;
	}
	else
		cout << "Error!!" << endl;
}

void GetHostByAddr()
{
	struct hostent *host;

	in_addr addr;
	inet_aton("216.58.192.227", &addr);

	host = gethostbyaddr((void *)&addr, 4, AF_INET);

	if (host != NULL)
	{
		cout << "Host name  :" << host->h_name << endl;
		cout << "Aliases  :";

		for (int i = 0; host->h_aliases[i] != NULL; i++)
			cout << "\t" << host->h_aliases[i];

		cout << endl;

		cout << "Address length  :" << host->h_length << endl;

		char str[INET_ADDRSTRLEN];

		cout << "IP Address list  :";
		for (int i = 0; host->h_addr_list[i] != NULL; i++)
			if (host->h_addrtype == AF_INET)
				cout << "\t"
					 << inet_ntop(AF_INET, host->h_addr_list[i], str,
								  INET_ADDRSTRLEN);

		cout << endl;
	}
	else
		cout << "Error!!" << endl;
}
