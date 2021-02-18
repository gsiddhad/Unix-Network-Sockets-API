#include <netdb.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
int main(int argc, char *argv[]) {
	int sid, n;
	char buffer[1024];
	struct sockaddr_in saddr;
	struct hostent *hen;
	if (argc < 3) {
		fprintf(stderr,”Error: Host name and port is not given \n”);
		exit(1);
	}
	sid = socket(AF_INET, SOCK_DGRAM, 0);
	if (sid < 0)
		perror("Socket create");
	hen = gethostbyname(argv[1]);
	if (hen == NULL) {
		fprintf(stdout, "Host not found");
		exit(1);
	}
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(atoi(argv[2]));
	bcopy((char *) hen->h_addr, (char *) &saddr.sin_addr.s_addr, hen->h_length);
	if (bind(sid, (struct sockaddr *) &saddr, sizeof(saddr)) < 0)
		perror("Socket_bind");
	printf("Enter the data u want to send : \n");
	fgets(buffer, 1023, stdin);
	n = sendto(sid, buffer, 1023, 0, (struct sockaddr *) &saddr, sizeof(saddr));
	if(n<0)
	perror(“Error in send to”);
	close(sid);
	return (0);
}
