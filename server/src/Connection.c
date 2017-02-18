#include "Connection.h"
#include "Packet.h"


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

		self->packet = new_Packet();

	} // ctor


	void Connection_dtor(Connection* self) {
		Connection_closeConnection(self);
	} // dtor


	void Connection_setLogger(Connection* self,Logger* logger) {
		self->logger = logger;
		Packet_setLogger(self->packet,logger);
	} // setLogger()


	int Connection_getSocket(Connection* self) {
		return self->socket;
	} // getSocket()


	void Connection_setSocket(Connection* self,int sock) {
printf("socket %d \n",sock);
		self->socket = sock;
	} // setSocket()


	void Connection_setSession(Connection* self,int session) {
		self->session = session;
		Packet_setSession(self->packet,session);
	} // setSession()


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

		Connection_setSession(self,session);

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
		self->socket = -1;
	} // closeConnection()


	// return: -1 - fail, other: okay
	int Connection_readPacket(Connection* self) {

		Packet_setLength(self->packet,0);

		int len = read(self->socket,Packet_getBuffer(self->packet),Packet_BUFLEN);

		if (len == -1) {
			Connection_log(self,Logger_NOTICE | Logger_DISPLAY,2014,"Error reading client socket, disconnecting");
			Connection_closeConnection(self);
			return -1;
		} // if read error

		if (len == 0) {
			Connection_log(self,Logger_NOTICE | Logger_DISPLAY,2015,"Client disconnected");
			Connection_closeConnection(self);
			return -1;
		} // if disconnect

		Packet_setLength(self->packet,len);

		return 0;
	} // readPacket()


	void Connection_process(Connection* self) {

		if ( Connection_readPacket(self) == -1 ) {
			Connection_closeConnection(self);
			return;
		}

		if ( Packet_process(self->packet) == -1 ) {
			// TODO: reply some error
			return;
		}

		// TODO: response
		send(self->socket,"OK\n",3,0);

		return; 
	} // process()


	