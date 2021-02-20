#include <unistd.h>	   /* standard unix functions, like getpid()       */
#include <sys/types.h> /* various type definitions, like pid_t         */
#include <signal.h>	   /* signal name macros, and the kill() prototype */
#include <stdio.h>
int main()
{
	/* first, find my own process ID */
	pid_t my_pid = getpid();
	printf("The process id of this process is : %d", my_pid);

	/* now that i got my PID, send myself the STOP signal. */
	kill(my_pid, SIGSTOP);
}
