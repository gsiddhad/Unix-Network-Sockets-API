#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <stdlib.h>
void Print_HostName();
void Print_IPaddr();
void R_Serv_Running();
void get_serv_name();
char hostname[190];
int main()
{
	int choice;
	while (1)
	{
		printf(
			" 1. HOst name \n 2. IP address \n 3. Are services running? \n 4. Get service name on port no. \n 5. Exit \n choice : ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			Print_HostName();
			break;
		case 2:
			Print_IPaddr();
			break;
		case 3:
			R_Serv_Running();
			break;
		case 4:
			get_serv_name();
			break;
		case 5:
			exit(1);
			break;
		default:
			printf("\n Enter a valid choice \n");
			break;
		}
		printf("\n\n");
	}

	return 0;
}

void Print_HostName()
{

	int success;
	success = gethostname(hostname, sizeof(hostname));
	if (success == 0)
	{
		printf("\n HOstName: ");
		printf("%s ", hostname);
		printf("\n\n");
	}
	else
		printf("Couldn't get hostname!");
}
void Print_IPaddr()
{
	struct hostent *hptr;
	char **pptr, ip_addr[50];
	hptr = gethostbyname(hostname);
	if (hptr != NULL)
	{
		pptr = hptr->h_addr_list;
		for (; *pptr != NULL; pptr++)
		{
			inet_ntop(hptr->h_addrtype, *pptr, ip_addr, sizeof(ip_addr));
			printf(" Address %s \n ", ip_addr);
			printf("\n\n");
		}
	}
}

void R_Serv_Running()
{
	struct servent *sptr;
	sptr = getservbyname("ftp", "tcp");

	if (sptr != NULL)
	{
		printf("\n FTP is running at port number: \n");
		printf("%d", sptr->s_port);
		printf("\n");
	}
	else
		printf("\n FTP is not running! \n");

	sptr = getservbyname("http", NULL);
	if (sptr != NULL)
	{
		printf("\n HTTP is running at port number:  \n");
		printf("%d", sptr->s_port);
		printf("\n");
	}
	else
		printf("\n HTTP is not running! \n");
	printf("\n");
}

void get_serv_name()
{
	struct servent *sptr;
	int portno;
	printf("\n Enter the port number\n");
	scanf("%d", &portno);
	sptr = getservbyport(portno, NULL);
	if (sptr != NULL)
	{
		printf("\n SErvice name \n");
		printf("%s", sptr->s_name);
		printf("\n");
	}
	else
		printf("\n no service running on this port ! \n");
}
