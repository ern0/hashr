#include "Server.h"


	Server* new_Server() {

		Server* self;
		self = (Server*)malloc(sizeof(Server));
		if (self != NULL) Server_ctor(self);

		return self;
	} // new 


	void delete_Server(Server* self) {

		Server_dtor(self);
		free(self);

	} // delete


	void Server_ctor(Server* self) {

		for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
			self->clientConnections[i] = NULL;
		}

		self->port = 8888;
		self->mainSocket = -1;
		self->logger = NULL;

	} // ctor()


	void Server_dtor(Server* self) {

		if (self->mainSocket != -1) close(self->mainSocket);

	} // dtor()


	void Server_setPort(Server* self,int port) {
		self->port = port;
	} // setPort()


	int Server_getPort(Server* self) {
		return self->port;
	} // getPort()


	void Server_setLogger(Server* self,Logger* logger) {
		self->logger = logger;
	} // setLogger()


	void Server_fatal(Server* self,int id,const char* message) {

		Logger_log(self->logger,Logger_FATAL,id,message);
		exit(2);

	} // fatal()


	void Server_start(Server* self) {

		self->mainSocket = socket(AF_INET,SOCK_STREAM,0);
		if (self->mainSocket == -1) {
			Server_fatal(self,1001,"socket failed");
		}

		int opt = 1;
		if ( -1 == setsockopt(self->mainSocket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,sizeof(opt)) ) {
			Server_fatal(self,1002,"setsockopt failed");
		}

		self->address.sin_family = AF_INET;
		self->address.sin_addr.s_addr = INADDR_ANY;
		self->address.sin_port = htons(self->port);

		if ( -1 == bind(self->mainSocket,(struct sockaddr *)&(self->address),sizeof(self->address))) { 
			Server_fatal(self,1003,"bind failed");
		}

		if ( -1 == listen(self->mainSocket,5) ) {
			Server_fatal(self,1004,"listen failed");
		}

		self->runningFlag = 1;
	     
	} // start()


	void Server_stop(Server* self) {

		self->runningFlag = 0;

	} // stop()


	void Server_run(Server* self) {

		while ( self->runningFlag ) {

			Server_buildFds(self);

 			struct timeval timeout;
 			timeout.tv_sec = 1;
 			timeout.tv_usec = 0;

			int sel = select(self->maxSocket + 1,&self->readfds,NULL,NULL,&timeout);
			if (sel == 0) continue;			
			
			if (sel == -1) {
				if (errno == EINTR) return;
				Server_fatal(self,1005,"select error");
			}

			if (FD_ISSET(self->mainSocket,&self->readfds)) {
				Server_connectNewClient(self);
			}	else {
				Server_handleOldClient(self);
			}
    
		} // while running

		close(self->mainSocket);

	} // run()


	void Server_buildFds(Server* self) {


		FD_ZERO(&self->readfds);
		FD_SET(self->mainSocket,&self->readfds);
		self->maxSocket = self->mainSocket;

		for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
			ClientConnection* conn = self->clientConnections[i];
			if (conn == NULL) continue;

			int sd = ClientConnection_getSocket(conn);
			if (sd == -1) continue;
			
			FD_SET(sd,&self->readfds);
			if (sd > self->maxSocket) self->maxSocket = sd;
		}	// for sockets

	} // buildFds()


	void Server_connectNewClient(Server* self) {

		int sock = accept(self->mainSocket,(struct sockaddr *)&self->address,(socklen_t*)&self->addrlen);
		if (sock == -1) Server_fatal(self,1006,"accept error");

		ClientConnection* incoming = new_ClientConnection();
		if (incoming == NULL) Server_fatal(self,1007,"no memory for new connection");

		ClientConnection_setLogger(incoming,self->logger);
		ClientConnection_setSocket(incoming,sock);

		for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
			if (self->clientConnections[i] != NULL) continue;

			self->clientConnections[i] = incoming;
			ClientConnection_acceptConnection(incoming);
			return;

		} // foreach slot

		ClientConnection_rejectConnection(incoming);
		delete_ClientConnection(incoming);

	} // connectNewClient()


	void Server_handleOldClient(Server* self) {

			for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {

				ClientConnection* conn = self->clientConnections[i];
				if (conn != NULL) continue;

				///self->clientConnections[i] = 4;
				break;

			} // foreach client connection

	} // handleOldClient()
