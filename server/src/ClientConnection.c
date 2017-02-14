#include "ClientConnection.h"


	ClientConnection* new_ClientConnection() {

		ClientConnection* self;
		self = (ClientConnection*)malloc(sizeof(ClientConnection));
		if (self != NULL) ClientConnection_ctor(self);

		return self;
	} // new 


	void delete_ClientConnection(ClientConnection* self) {

		ClientConnection_dtor(self);
		free(self);

	} // delete 


	void ClientConnection_ctor(ClientConnection* self) {

		// NOP

	} // ctor


	void ClientConnection_dtor(ClientConnection* self) {

		// NOP

	} // dtor



	int ClientConnection_getSocket(ClientConnection* self) {
		return self->clientSocket;
	} // getSocket()
