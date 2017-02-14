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

		self->socket = -1;

	} // ctor


	void ClientConnection_dtor(ClientConnection* self) {

		if (self->socket != -1) close(self->socket);

	} // dtor


	void ClientConnection_setLogger(ClientConnection* self,Logger* logger) {
		self->logger = logger;
	} // setLogger()


	int ClientConnection_getSocket(ClientConnection* self) {
		return self->socket;
	} // getSocket()


	void ClientConnection_setSocket(ClientConnection* self,int sock) {
		self->socket = sock;
	} // setSocket()


	void Client_fatal(ClientConnection* self,int id,const char* message) {

		Logger_log(self->logger,Logger_FATAL,id,message);
		exit(2);

	} // fatal()


	void ClientConnection_acceptConnection(ClientConnection* self) {

		Logger_log(
			self->logger
			,Logger_NOTICE | Logger_DISPLAY
			,2001,"Accepting connection"
		);

		/// todo

	} // acceptConnection()


	void ClientConnection_rejectConnection(ClientConnection* self) {

		Logger_log(
			self->logger
			,Logger_ERROR | Logger_DISPLAY
			,2011,"No more connections available, rejecting"
		);

		const char* message = "REJECT\n";
		int sent = send(self->socket,message,strlen(message),0);
		if (sent == -1) Logger_log(self->logger,Logger_ERROR,2012,"client send failed");

		/// todo

	} // rejectConnection()

