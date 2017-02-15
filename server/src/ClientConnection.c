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
		self->session = 0;
	} // ctor


	void ClientConnection_dtor(ClientConnection* self) {
		ClientConnection_closeConnection(self);
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


	void ClientConnection_log(ClientConnection* self,int level,int id,const char* message) {

		char extendedMessage[200];
		strncpy(extendedMessage,message,199);
		strncat(extendedMessage," (session=%d)",199);

		Logger_logi(self->logger,level,id,extendedMessage,self->session);

	} // log()


	void Client_fatal(ClientConnection* self,int id,const char* message) {
		ClientConnection_log(self,Logger_FATAL,id,message);
		exit(2);
	} // fatal()


	void ClientConnection_acceptConnection(ClientConnection* self,int session) {

		self->session = session;

		ClientConnection_log(self,Logger_NOTICE | Logger_DISPLAY,2001,"Accepting connection");

		char message[20];
		sprintf(message,"ACCEPT %d\n",self->session);
		int sent = send(self->socket,message,strlen(message),0);
		if (sent == -1) {
			ClientConnection_log(self,Logger_ERROR,2011,"client send failed");
			return;
		} // if send fail

		ClientConnection_log(self,Logger_DEBUG,2012,"Connection accepted");

	} // acceptConnection()


	void ClientConnection_rejectConnection(ClientConnection* self) {

		Logger_log(self->logger,Logger_ERROR | Logger_DISPLAY,2011,"No more connections available, rejecting");

		const char* message = "REJECT\n";
		int sent = send(self->socket,message,strlen(message),0);
		if (sent == -1) ClientConnection_log(self,Logger_ERROR,2002,"client send failed");

	} // rejectConnection()


	void ClientConnection_closeConnection(ClientConnection* self) {
		if (self->socket != -1) close(self->socket);
	} // closeConnection()


	int ClientConnection_process(ClientConnection* self) {

		int len = read(self->socket,self->buffer,ClientConnection_BUFLEN);

		if (len == -1) {
			ClientConnection_log(self,Logger_NOTICE | Logger_DISPLAY,2014,"Error reading client socket, disconnecting");
			ClientConnection_closeConnection(self);
			return 0;
		} // if read error

		if (len == 0) {
			ClientConnection_log(self,Logger_NOTICE | Logger_DISPLAY,2015,"Client disconnected");
			ClientConnection_closeConnection(self);
			return 0;
		} // if disconnect

		self->buffer[len - 1] = 0;
		printf("proc started session=%d: [%s] \n",self->session,self->buffer);

		send(self->socket,"OK...\n",6,0);
		sleep(2);
		send(self->socket,"...OK\n",6,0);

		printf("proc finished session=%d: [%s] \n",self->session,self->buffer);

		return 1;
	} // process()

