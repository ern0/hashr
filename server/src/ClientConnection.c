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
		self->chunkCount = -1;

		ClientConnection_setBuffer(self,self->internalBuffer,-1);

	} // ctor


	void ClientConnection_dtor(ClientConnection* self) {
		ClientConnection_closeConnection(self);
	} // dtor


	void ClientConnection_setBuffer(ClientConnection* self,unsigned char* buffer,int len) {
		self->buffer = buffer;
		self->len = len;
	} // setBuffer()


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
		sprintf(message,"%d\n",self->session);
		int sent = send(self->socket,message,strlen(message),0);
		if (sent == -1) {
			ClientConnection_log(self,Logger_ERROR,2011,"client send failed");
			return;
		} // if send fail

		ClientConnection_log(self,Logger_DEBUG,2012,"Connection accepted");

	} // acceptConnection()


	void ClientConnection_rejectConnection(ClientConnection* self) {

		Logger_log(self->logger,Logger_ERROR | Logger_DISPLAY,2011,"No more connections available, rejecting");

		const char* message = "0\n";
		int sent = send(self->socket,message,strlen(message),0);
		if (sent == -1) ClientConnection_log(self,Logger_ERROR,2002,"client send failed");

	} // rejectConnection()


	void ClientConnection_closeConnection(ClientConnection* self) {
		if (self->socket != -1) close(self->socket);
	} // closeConnection()


	// return: 0 - okay, other - fail
	int ClientConnection_readPacket(ClientConnection* self) {

		self->len = read(self->socket,self->buffer,ClientConnection_BUFLEN);

		if (self->len == -1) {
			ClientConnection_log(self,Logger_NOTICE | Logger_DISPLAY,2014,"Error reading client socket, disconnecting");
			ClientConnection_closeConnection(self);
			return -1;
		} // if read error

		if (self->len == 0) {
			ClientConnection_log(self,Logger_NOTICE | Logger_DISPLAY,2015,"Client disconnected");
			ClientConnection_closeConnection(self);
			return -1;
		} // if disconnect

		return 0;
	} // readPacket()


	int ClientConnection_isHeaderOk(ClientConnection* self) {

		if ( self->buffer[0] != 'H' ) return 0;
		if ( self->buffer[1] != 'S' ) return 0;
		if ( self->buffer[2] != 'H' ) return 0;
		if ( self->buffer[3] != 'r' ) return 0;

		return 1;
	} // isHeaderOk()


	int ClientConnection_scanChunks(ClientConnection* self) {

		int result = 0;
		int index = 4;

		while (1) {

			if (self->len < index + 4) return -1;
			
			if (Utils_isEqSigs(&self->buffer[index],(const unsigned char*)"endm")) {
				break;
			}

			int chunkLength = Utils_getBufInt(&self->buffer[index + 4]);
			if (chunkLength < 0) return -1;

			result++;
			index += chunkLength + 8;
					
		} // loop

		return result;
	} // scanChunks()


	int ClientConnection_findChunk(ClientConnection* self,unsigned char* sig) {

		int index = 4;

		while (1) {

			if (Utils_isEqSigs(&self->buffer[index],(const unsigned char*)"endm")) {
				return -1;
			}

			if (Utils_isEqSigs(&self->buffer[index],sig)) break;

			int chunkLength = Utils_getBufInt(&self->buffer[index + 4]);
			index += chunkLength + 8;
					
		} // loop

		return index + 8;
	} // findChunks()


	int ClientConnection_processPacket(ClientConnection* self) {

		if (!ClientConnection_isHeaderOk(self)) {
			ClientConnection_log(self,Logger_ERROR | Logger_DISPLAY,2018,"Invalid request");
			return -1;		
		} // if invalid request

		self->chunkCount = ClientConnection_scanChunks(self);
		if (self->chunkCount == -1) {
			ClientConnection_log(self,Logger_ERROR | Logger_DISPLAY,2019,"Damaged request");
			return -1;		
		} // if damaged request
	
		return 0;
	} // processPacket()


	void ClientConnection_processRequest(ClientConnection* self) {

		if ( ClientConnection_readPacket(self) ) {
			ClientConnection_closeConnection(self);
			return;
		}

		if (-1 == ClientConnection_processPacket(self)) {
			return;
		}

		// TODO: response

		send(self->socket,"OK\n",3,0);

	} // processRequest()


	