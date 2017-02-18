#include "Connection.h"


	Connection* new_Connection() {

		Connection* self;
		self = (Connection*)malloc(sizeof(Connection));
		if (self != NULL) Connection_ctor(self);

		return self;
	} // new 


	void delete_Connection(Connection* self) {
		Connection_dtor(self);
		free(self);
	} // delete 


	void Connection_ctor(Connection* self) {

		self->socket = -1;
		self->session = 0;
		self->chunkCount = -1;

		Connection_setBuffer(self,self->internalBuffer,-1);

	} // ctor


	void Connection_dtor(Connection* self) {
		Connection_closeConnection(self);
	} // dtor


	void Connection_setBuffer(Connection* self,unsigned char* buffer,int len) {
		self->buffer = buffer;
		self->len = len;
	} // setBuffer()


	void Connection_setLogger(Connection* self,Logger* logger) {
		self->logger = logger;
	} // setLogger()


	int Connection_getSocket(Connection* self) {
		return self->socket;
	} // getSocket()


	void Connection_setSocket(Connection* self,int sock) {
		self->socket = sock;
	} // setSocket()


	void Connection_log(Connection* self,int level,int id,const char* message) {

		char extendedMessage[200];
		strncpy(extendedMessage,message,199);
		strncat(extendedMessage," (session=%d)",199);

		Logger_logi(self->logger,level,id,extendedMessage,self->session);

	} // log()


	void Connection_fatal(Connection* self,int id,const char* message) {
		Connection_log(self,Logger_FATAL,id,message);
		exit(2);
	} // fatal()


	void Connection_acceptConnection(Connection* self,int session) {

		self->session = session;

		Connection_log(self,Logger_NOTICE | Logger_DISPLAY,2001,"Accepting connection");

		char message[20];
		sprintf(message,"%d\n",self->session);
		int sent = send(self->socket,message,strlen(message),0);
		if (sent == -1) {
			Connection_log(self,Logger_ERROR,2011,"client send failed");
			return;
		} // if send fail

		Connection_log(self,Logger_DEBUG,2012,"Connection accepted");

	} // acceptConnection()


	void Connection_rejectConnection(Connection* self) {

		Logger_log(self->logger,Logger_ERROR | Logger_DISPLAY,2011,"No more connections available, rejecting");

		const char* message = "0\n";
		int sent = send(self->socket,message,strlen(message),0);
		if (sent == -1) Connection_log(self,Logger_ERROR,2002,"client send failed");

	} // rejectConnection()


	void Connection_closeConnection(Connection* self) {
		if (self->socket != -1) close(self->socket);
	} // closeConnection()


	// return: 0 - okay, other - fail
	int Connection_readPacket(Connection* self) {

		self->len = read(self->socket,self->buffer,Connection_BUFLEN);

		if (self->len == -1) {
			Connection_log(self,Logger_NOTICE | Logger_DISPLAY,2014,"Error reading client socket, disconnecting");
			Connection_closeConnection(self);
			return -1;
		} // if read error

		if (self->len == 0) {
			Connection_log(self,Logger_NOTICE | Logger_DISPLAY,2015,"Client disconnected");
			Connection_closeConnection(self);
			return -1;
		} // if disconnect

		return 0;
	} // readPacket()


	int Connection_isHeaderOk(Connection* self) {

		if ( self->buffer[0] != 'H' ) return 0;
		if ( self->buffer[1] != 'S' ) return 0;
		if ( self->buffer[2] != 'H' ) return 0;
		if ( self->buffer[3] != 'r' ) return 0;

		return 1;
	} // isHeaderOk()


	int Connection_scanChunks(Connection* self) {

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


	int Connection_findChunk(Connection* self,unsigned char* sig) {

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


	int Connection_processPacket(Connection* self) {

		if (!Connection_isHeaderOk(self)) {
			Connection_log(self,Logger_ERROR | Logger_DISPLAY,2018,"Invalid request");
			return -1;		
		} // if invalid request

		self->chunkCount = Connection_scanChunks(self);
		if (self->chunkCount == -1) {
			Connection_log(self,Logger_ERROR | Logger_DISPLAY,2019,"Damaged request");
			return -1;		
		} // if damaged request
	
		return 0;
	} // processPacket()


	void Connection_processRequest(Connection* self) {

		if ( Connection_readPacket(self) ) {
			Connection_closeConnection(self);
			return;
		}

		if (-1 == Connection_processPacket(self)) {
			return;
		}

		// TODO: response

		send(self->socket,"OK\n",3,0);

	} // processRequest()


	