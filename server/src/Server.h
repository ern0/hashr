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

#include "Logger.h"
#include "Connection.h"


#define MAX_CLIENT_NUMBER 2


// class Server

	// private
	struct Server {

		Logger* logger;
		int port;
		int runningFlag;
		int session;

		int addrlen;
		int mainSocket;
		int maxSocket;
		struct sockaddr_in address;

		fd_set readfds;	

		char buffer[1025];
		Connection* connections[MAX_CLIENT_NUMBER];		
	};
	typedef struct Server Server;

	// public
	Server* new_Server();
	void delete_Server(Server* self);
	void Server_setLogger(Server* self,Logger* logger);
	void Server_setPort(Server* self,int port);
	int Server_getPort(Server* self);
	void Server_start(Server* self);
	void Server_stop(Server* self);
	void Server_run(Server* self);

	// protected
	void Server_ctor(Server* self);
	void Server_dtor(Server* self);
	void Server_fatal(Server* self,int id,const char* message);
	void Server_buildFds(Server* self);
	void Server_connectNewClient(Server* self);
	void Server_handleOldClient(Server* self);

#endif