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


	void Server_fatal(Server* self,const char* message) {

	} // fatal()


	void Server_start(Server* self) {

		self->mainSocket = socket(AF_INET,SOCK_STREAM,0);
		if (self->mainSocket == -1) {
			Server_fatal(self,"socket failed");
		}

		int opt = 1;
		if ( -1 == setsockopt(self->mainSocket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,sizeof(opt)) ) {
			Server_fatal(self,"setsockopt failed");
		}

		self->address.sin_family = AF_INET;
		self->address.sin_addr.s_addr = INADDR_ANY;
		self->address.sin_port = htons(self->port);

		if ( -1 == bind(self->mainSocket,(struct sockaddr *)&(self->address),sizeof(self->address))) { 
			Server_fatal(self,"bind failed");
		}

		if ( -1 == listen(self->mainSocket,5) ) {
			Server_fatal(self,"listen failed");
		}

		self->runningFlag = 1;
	     
	} // start()


	void Server_stop(Server* self) {

		self->runningFlag = 0;

	} // stop()


	void Server_run(Server* self) {

		while ( self->runningFlag ) {

			Server__buildFds(self);

 			struct timeval timeout;
 			timeout.tv_sec = 1;
 			timeout.tv_usec = 0;

			int sel = select(self->maxSocket + 1,&self->readfds,NULL,NULL,&timeout);
			if (sel == 0) continue;			
			if (sel == -1) {
				if (errno == EINTR) return;
				Server_fatal(self,"select error");
			}
    
		} // while running

		close(self->mainSocket);

	} // run()


	void Server__buildFds(Server* self) {


		FD_ZERO(&self->readfds);
		FD_SET(self->mainSocket,&self->readfds);
		self->maxSocket = self->mainSocket;

		for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
			ClientConnection* client = self->clientConnections[i];
			if (client == NULL) continue;

			int sd = ClientConnection_getSocket(client);
			if (sd == -1) continue;
			
			FD_SET(sd,&self->readfds);
			if (sd > self->maxSocket) self->maxSocket = sd;
		}	// for sockets

	} // buildFds()

