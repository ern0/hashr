#ifndef Server_h_included
#define Server_h_included

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>


// class Server

	#define MAX_CLIENT_NUMBER 20

	// private
	struct Server {
		int port;
		int addrlen;
		int mainSocket;
		int maxSocket;
		int clientSockets[MAX_CLIENT_NUMBER];
		struct sockaddr_in address;

		char buffer[1025];
		fd_set readfds;		
	};
	typedef struct Server Server;

	// public
	void Server_ctor(Server* self);
	void Server_dtor(Server* self);
	void Server_setPort(Server* self,int port);
	int Server_getPort(Server* self);
	void Server_prepare(Server* self);
	void Server_run(Server* self);

	// protected
	void Server__buildFds(Server* self);

#endif