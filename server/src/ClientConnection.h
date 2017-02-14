#ifndef ClientConnection_h_included
#define ClientConnection_h_included


// class ClientConnection

	// private
	struct ClientConnection {
		int clientSocket;
	};
	typedef struct ClientConnection ClientConnection;


	// public
	void ClientConnection_ctor(ClientConnection* self);
	int ClientConnection_getSocket(ClientConnection* self);



#endif