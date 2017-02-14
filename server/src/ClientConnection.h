#ifndef ClientConnection_h_included
#define ClientConnection_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "Logger.h"


// class ClientConnection

	struct ClientConnection {
		Logger* logger;
		int socket;
	};
	typedef struct ClientConnection ClientConnection;

	// public
	ClientConnection* new_ClientConnection();
	void delete_ClientConnection(ClientConnection* self);
	void ClientConnection_setLogger(ClientConnection* self,Logger* logger);
	int ClientConnection_getSocket(ClientConnection* self);
	void ClientConnection_setSocket(ClientConnection* self,int sock);
	void ClientConnection_acceptConnection(ClientConnection* self);	
	void ClientConnection_rejectConnection(ClientConnection* self);

	// protected
	void ClientConnection_ctor(ClientConnection* self);
	void ClientConnection_dtor(ClientConnection* self);
	void ClientConnection_fatal(ClientConnection* self,int id,const char* message);


#endif