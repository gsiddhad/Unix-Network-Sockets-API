#include <sys/socket.h>
#include <strings.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> //err no
#include <signal.h>
#include <sys/wait.h> //waitpid()
#include <sys/resource.h>
#include <arpa/inet.h> // for inet_ntop

void sig_chld(int signo);
void str_echo(int sockfd);
ssize_t readn(int fd, void *vptr, size_t n);
ssize_t writen(int fd, const void *vptr, size_t n);
void sighandler(int signum);
void handler(int s);
static int c;

int main(int argc, char **argv)
{
	int listenfd, connfd;
	char buff[1000];
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		exit(1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(9944);

	if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind");
		exit(1);
	}
	if (listen(listenfd, 5) < 0)
	{
		perror("listen");
		exit(1);
	}
	signal(SIGPIPE, handler);
	signal(SIGCHLD, sig_chld);
	signal(SIGSEGV, sighandler);
	/* must call waitpid() */
	for (;;)
	{
		puts("\n*****server waiting for new client connection:*****\n");
		clilen = sizeof(cliaddr);
		if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
		{
			if (errno == EINTR)
				continue; /* back to for */
			else
			{
				perror("accept");
				exit(1);
			}
		}
		c = c + 1;
		printf("connected to client: %s\n",
			   inet_ntop(AF_INET, (struct addr *)&cliaddr.sin_addr, buff,
						 sizeof(buff)));
		printf("You are client no : %d \n", c);

		if ((childpid = fork()) < 0)
		{
			perror("fork");
			exit(1);
		}
		else if (childpid == 0) /* child process */
		{
			if (close(listenfd) < 0) /* close listening socket */
			{
				perror("child close");
				exit(1);
			}
			str_echo(connfd); /* process the request */
			exit(0);
		}
		if (close(connfd) < 0) /* parent close connected socket */
		{
			perror("parent close");
			exit(1);
		}
	}
}

void handler(int s)
{
	printf("Caught SIGPIPE\n");
}

void sig_chld(int signo)
{
	pid_t pid;
	int stat;

	while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}
void sighandler(int signum)
{
	printf("Process %d got signal %d\n", getpid(), signum);
	signal(signum, SIG_DFL);
	kill(getpid(), signum);
}
void str_echo(int sockfd)
{
	ssize_t n;
	char buf[4096];
again:
	while ((n = read(sockfd, buf, 4096)) > 0)
	{
		writen(sockfd, buf, 1);
		pause();
		writen(sockfd, buf, n - 1);
	}

	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
	{
		perror("read");
		exit(1);
	}
}
ssize_t readn(int fd, void *vptr, size_t n) /* read "n" bytes from a descriptor. */
{
	size_t nleft;
	ssize_t nread;
	char *ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0)
	{
		if ((nread = read(fd, ptr, nleft)) < 0)
		{
			if (errno == EINTR)
				continue; /* and call read() again */
			else
				return (-1);
		}
		else if (nread == 0)
			break; /* EOF */

		nleft -= nread;
		ptr += nread;
	}
	return (n - nleft); /* return >= 0 */
}

ssize_t writen(int fd, const void *vptr, size_t n) /* write n bytes to a descriptor */
{
	size_t nleft;
	ssize_t nwritten;
	const char *ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0)
	{
		if ((nwritten = write(fd, ptr, nleft)) <= 0)
		{
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0; /* and call write again */
			else
				return (-1); /* error */
		}

		nleft -= nwritten;
		ptr += nwritten;
	}
	return (n - nwritten);
}

ssize_t readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t n, rc;
	char c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++)
	{
	again:
		if ((rc = read(fd, &c, 1)) == 1)
		{
			*ptr++ = c;
			if (c == '\n')
				break; /* newline is stored, like fgets() */
		}
		else if (rc == 0)
		{
			*ptr = 0;
			return (n - 1); /* EOF, n - 1 bytes were read */
		}
		else
		{
			if (errno == EINTR)
				goto again;
			return (-1); /* error, errno set by read() */
		}
	}

	*ptr = 0; /* null terminate like fgets() */
	return (n);
}
