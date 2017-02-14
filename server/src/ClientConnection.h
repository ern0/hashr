#ifndef ClientConnection_h_included
#define ClientConnection_h_included

#include <malloc.h>
#include <stdlib.h>


// class ClientConnection

	// private
	struct ClientConnection {
		int clientSocket;
	};
	typedef struct ClientConnection ClientConnection;


	// public
	ClientConnection* new_ClientConnection();
	void delete_ClientConnection(ClientConnection* self);
	int ClientConnection_getSocket(ClientConnection* self);

	// protected
	void ClientConnection_ctor(ClientConnection* self);
	void ClientConnection_dtor(ClientConnection* self);


#endif