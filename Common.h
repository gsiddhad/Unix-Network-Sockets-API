// Common.h: Header file for common functions

#include<iostream>
#include<string>
#include<time.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<errno.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/wait.h>

using namespace std;

/*********************** Insert Null At end of line*****************/
void ReplaceNewineWithNull(char *Str, size_t Size) {
	for (size_t i = 0; i < Size; ++i) {
		if (Str[i] == '\r' || Str[i] == '\n') {
			Str[i] = '\0';
			break;
		}
	}
}
/************************** Error Handler ***************************/
void CheckError(const int return_value, const char *function,
		bool exit_process = true) {
	if (return_value < 0) {
		cout << "\n ERROR!!!";
		cout << "\n Function: " << function;
		cout << "\n Error Code: " << errno;
		cout << "\n Description: " << strerror(errno) << endl;
		fflush(stdout);

		if (exit_process)
			exit(1);
	}
}

/************************ Echo Client ***************************/
int TCP_EchoClient(int server_desc) {
	const int BUFFER_SIZE = 4096;
	char msg[BUFFER_SIZE];
	int len = 0;

	while (true) {
		cout << "\n Input : ";
		fflush(stdout);
		len = read(fileno(stdin), msg, BUFFER_SIZE);
		CheckError(len, "read()");

		if (len == 0) //EOF
			return 0;

		len = write(server_desc, msg, len);
		CheckError(len, "write()");

		len = read(server_desc, msg, BUFFER_SIZE);
		CheckError(len, "read()");

		cout << " Responce From Server :";
		fflush(stdout);
		len = write(fileno(stdout), msg, len);
		CheckError(len, "write()");
	}
	return 0;
}

/************************ Echo Server ***************************/
int TCP_EchoServer(int client_desc) {
	const int BUFFER_SIZE = 4096;
	char msg[BUFFER_SIZE];
	int msg_len = 0;

	while ((msg_len = read(client_desc, msg, BUFFER_SIZE)) != 0) {
		if (msg_len < 0)  //Error
			return msg_len;

		for (int i = 0; i < msg_len; ++i)
			cout << "\n[" << i << "] (" << int(msg[i]) << ") " << msg[i];

		write(client_desc, msg, msg_len);
	}

	// EOF
	return 0;
}

/************************ Chat Client ***************************/
int TCP_ChatClient(int server_desc) {
	const int BUFFER_SIZE = 4096;
	char msg[BUFFER_SIZE];
	int len = 0;

	while (true) {
		cout << "\n Input : ";
		fflush(stdout);
		len = read(fileno(stdin), msg, BUFFER_SIZE);
		CheckError(len, "read()");

		if (len == 0) //EOF
			return 0;

		len = write(server_desc, msg, len);
		CheckError(len, "write()");

		if (msg[0] == 'b' && msg[1] == 'y' && msg[2] == 'e')
			return 0;

		len = read(server_desc, msg, BUFFER_SIZE);
		CheckError(len, "read()");

		cout << " Responce From Server :";
		fflush(stdout);
		len = write(fileno(stdout), msg, len);
		CheckError(len, "write()");

		if (msg[0] == 'b' && msg[1] == 'y' && msg[2] == 'e')
			return 0;

	}
	return 0;
}

/************************ Chat Server ***************************/
int TCP_ChatServer(int client_desc) {
	const int BUFFER_SIZE = 4096;
	char msg[BUFFER_SIZE];
	int msg_len = 0;

	while ((msg_len = read(client_desc, msg, BUFFER_SIZE)) != 0) {
		cout << "[User " << client_desc << "] Msg Recieved :";
		fflush(stdout);
		write(fileno(stdout), msg, msg_len);

		if (msg[0] == 'b' && msg[1] == 'y' && msg[2] == 'e')
			return 0;

		cout << "[User " << client_desc << "] Enter Responce :";
		fflush(stdout);
		msg_len = read(fileno(stdin), msg, BUFFER_SIZE);

		write(client_desc, msg, msg_len);

		if (msg[0] == 'b' && msg[1] == 'y' && msg[2] == 'e')
			return 0;
	}

	// EOF
	return 0;
}

/************************ File Server (TCP) ***************************/
int TCP_FileServer(int client_desc) {
	const int BUFFER_SIZE = 4096;
	char msg[BUFFER_SIZE];
	int msg_len = 0;

	//Read a file name
	msg_len = read(client_desc, msg, BUFFER_SIZE);
	CheckError(msg_len, "read()");

	//Try to open file
	FILE *file = fopen(msg, "r");

	if (file != NULL) {
		//Send a responce to indicate success
		msg[0] = 1;
		write(client_desc, msg, 1);

		//Start sending file data
		while ((msg_len = read(fileno(file), msg, BUFFER_SIZE)) != 0) {
			CheckError(msg_len, "read()");
			write(client_desc, msg, msg_len);
		}
	} else {
		//Send a responce to indicate failure
		msg[0] = 0;
		write(client_desc, msg, 1);
	}

	return 0;
}

/************************ File Client (TCP) ***************************/
int TCP_FileClient(int server_desc, char *FileName) {
	const int BUFFER_SIZE = 4096;
	char Buff[BUFFER_SIZE];

	//Send file Name to Server
	write(server_desc, FileName, strlen(FileName));

	//Read responce
	int Count = read(server_desc, Buff, 1);
	CheckError(Count, "read()");

	if (Count > 0) {
		if (Buff[0] == 1)        //Success
				{
			//Open file for writing
			strcpy(Buff, "Doweloaded_");
			strcat(Buff, FileName);
			FILE *file = fopen(Buff, "w");

			if (file != NULL) {
				//Download file
				while ((Count = read(server_desc, Buff, BUFFER_SIZE)) != 0) {
					CheckError(Count, "read()");
					write(fileno(file), Buff, Count);
				}

				cout << "\n Download complete";
				fclose(file);
			}
		} else
			cout << "\nFile not found or  Access Denied: ";
		fflush(stdout);
	} else
		cout << "Server Terminated without Responce";

	return 0;
}

/************************* Address Family **************************/
const char* StrAddressFamily(int af) {
	switch (af) {
	case AF_INET:
		return "AF_INET";
	case AF_INET6:
		return "AF_INET6";
	case AF_LOCAL:
		return "AF_LOCAL";
	case AF_ROUTE:
		return "AF_ROUTE";
	case AF_KEY:
		return "AF_KEY";
	}
	return "Unknown";
}

/************************ DayTime Server (TCP)****************************/
void TCP_DayTimeServer(int ClientSock) {
	time_t millis;
	time(&millis);                   //Get time in millis

	tm *t = localtime(&millis);      //Break millis into days, hours ...

	char Str[200];

	strftime(Str, 199, "%c", t);      // Convert into string

	write(ClientSock, Str, strlen(Str)); //Send to client

}

/************************ DayTime Server (UDP)****************************/
void UDP_DayTimeServer(int ServerSock, sockaddr_in &ClientAddr,
		socklen_t ClientAddrLen) {
	time_t millis;
	time(&millis);                   //Get time in millis

	tm *t = localtime(&millis);      //Break millis into days, hours ...

	char Str[200];

	strftime(Str, 199, "%c", t);      // Convert into string

	sendto(ServerSock, Str, strlen(Str), 0, (sockaddr*) &ClientAddr,
			ClientAddrLen); //Send to client

}

