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


	int ClientConnection_process(ClientConnection* self) {

		self->len = read(self->socket,self->buffer,ClientConnection_BUFLEN);

		if (self->len == -1) {
			ClientConnection_log(self,Logger_NOTICE | Logger_DISPLAY,2014,"Error reading client socket, disconnecting");
			ClientConnection_closeConnection(self);
			return 0;
		} // if read error

		if (self->len == 0) {
			ClientConnection_log(self,Logger_NOTICE | Logger_DISPLAY,2015,"Client disconnected");
			ClientConnection_closeConnection(self);
			return 0;
		} // if disconnect

		printf("processing request, session=%d \n",self->session);

		ClientConnection_dumpBuffer(self);
		send(self->socket,"OK\n",3,0);

		return 1;
	} // process()


	void ClientConnection_dumpBuffer(ClientConnection* self) {

		int ptr = 0;

		while (1) {
			if (ptr >= self->len) break;

			printf("  %04X: ",ptr);

			for (int i = 0; i < 8; i++) {
				int index = ptr + i;

				if (index < self->len) {
					unsigned char b = self->buffer[ptr + i];
					printf("%02X ",b);
				} // if payload

				else {
					printf("   ");
				} // else fill

			} // for numbers

			printf(" \e[7m");

			for (int i = 0; i < 8; i++) {
				int index = ptr + i;

				if (index < self->len) {
					unsigned char b = self->buffer[ptr + i];
					if (b < ' ') b = '.';
					if (b > 126) b = '.';
					printf("%c",b);
				} // if payload

			} // for characters

			printf("\e[0m\n");

			ptr += 8;
		} // while buffer

	} // dumpBuffer()
