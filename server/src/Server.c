#include "Server.h"
#include "Utils.h"


	void Server_ctor(Server* self) {

		for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
			self->clientSockets[i] = -1;
		}

		self->port = 8888;
		self->mainSocket = -1;

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


	void Server_prepare(Server* self) {

		self->mainSocket = socket(AF_INET,SOCK_STREAM,0);
		if (self->mainSocket == -1) {
			Utils_fatal("socket failed");
		}

		int opt = 1;
		if ( -1 == setsockopt(self->mainSocket,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,sizeof(opt)) ) {
			Utils_fatal("setsockopt failed");
		}

		self->address.sin_family = AF_INET;
		self->address.sin_addr.s_addr = INADDR_ANY;
		self->address.sin_port = htons(self->port);

		if ( -1 == bind(self->mainSocket,(struct sockaddr *)&(self->address),sizeof(self->address))) { 
			Utils_fatal("bind failed");
		}

		if ( -1 == listen(self->mainSocket,5) ) {
			Utils_fatal("listen failed");
		}
	     
	} // prepare()


	void Server_run(Server* self) {

		while (1) {
			Server__buildFds(self);
			break;
		}

	} // run()


	void Server__buildFds(Server* self) {


		FD_ZERO(&self->readfds);
		FD_SET(self->mainSocket,&self->readfds);
		self->maxSocket = self->mainSocket;

		for (int i = 0; i < MAX_CLIENT_NUMBER; i++) {
			int sd = self->clientSockets[i];
			
			if (sd != -1) FD_SET(sd,&self->readfds);
			if (sd > self->maxSocket) self->maxSocket = sd;
		}	// for sockets

	} // buildFds()

