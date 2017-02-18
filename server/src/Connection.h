#ifndef Connection_h_included
#define Connection_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include "Utils.h"
#include "Logger.h"


// class Connection

	// const
	#define Connection_BUFLEN 20000
	
	// private
	struct Connection {
		int session;
		Logger* logger;
		int socket;
		unsigned char* buffer;
		unsigned char internalBuffer[Connection_BUFLEN];
		int len;
		int chunkCount;
	};
	typedef struct Connection Connection;

	// test
	void Connection_setBuffer(Connection* self,unsigned char* buffer,int len);

	// public
	Connection* new_Connection();
	void delete_Connection(Connection* self);
	void Connection_setLogger(Connection* self,Logger* logger);
	void Connection_log(Connection* self,int level,int id,const char* message);
	int Connection_getSocket(Connection* self);
	void Connection_setSocket(Connection* self,int sock);
	void Connection_acceptConnection(Connection* self,int session);	
	void Connection_rejectConnection(Connection* self);
	void Connection_closeConnection(Connection* self);
	void Connection_processRequest(Connection* self);

	// protected
	void Connection_ctor(Connection* self);
	void Connection_dtor(Connection* self);
	void Connection_fatal(Connection* self,int id,const char* message);
	int Connection_readPacket(Connection* self);
	int Connection_processPacket(Connection* self);	
	int Connection_isHeaderOk(Connection* self);
	int Connection_scanChunks(Connection* self);
	int Connection_findChunk(Connection* self,unsigned char* sig);

#endif