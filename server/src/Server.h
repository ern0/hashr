#ifndef Server_h_included
#define Server_h_included

#include <malloc.h>
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

#include "ClientConnection.h"

// class Server

	#define MAX_CLIENT_NUMBER 20

	// private
	struct Server {

		int port;
		int runningFlag;

		int addrlen;
		int mainSocket;
		int maxSocket;
		struct sockaddr_in address;

		fd_set readfds;	

		char buffer[1025];
		ClientConnection* clientConnections[MAX_CLIENT_NUMBER];		
	};
	typedef struct Server Server;

	// public
	Server* new_Server();
	void delete_Server(Server* self);
	void Server_setPort(Server* self,int port);
	int Server_getPort(Server* self);
	void Server_start(Server* self);
	void Server_stop(Server* self);
	void Server_run(Server* self);

	// protected
	void Server_ctor(Server* self);
	void Server_dtor(Server* self);
	void Server__buildFds(Server* self);

#endif