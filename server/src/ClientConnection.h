#ifndef ClientConnection_h_included
#define ClientConnection_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "Logger.h"


// class ClientConnection

	// const
	#define ClientConnection_BUFLEN 20000
	
	// private
	struct ClientConnection {
		int session;
		Logger* logger;
		int socket;
		char buffer[ClientConnection_BUFLEN];
		int len;
	};
	typedef struct ClientConnection ClientConnection;

	// public
	ClientConnection* new_ClientConnection();
	void delete_ClientConnection(ClientConnection* self);
	void ClientConnection_setLogger(ClientConnection* self,Logger* logger);
	void ClientConnection_log(ClientConnection* self,int level,int id,const char* message);
	int ClientConnection_getSocket(ClientConnection* self);
	void ClientConnection_setSocket(ClientConnection* self,int sock);
	void ClientConnection_acceptConnection(ClientConnection* self,int session);	
	void ClientConnection_rejectConnection(ClientConnection* self);
	void ClientConnection_closeConnection(ClientConnection* self);
	int ClientConnection_process(ClientConnection* self);

	// protected
	void ClientConnection_ctor(ClientConnection* self);
	void ClientConnection_dtor(ClientConnection* self);
	void ClientConnection_fatal(ClientConnection* self,int id,const char* message);
	void ClientConnection_dumpBuffer(ClientConnection* self);


#endif