#ifndef Connection_h_included
#define Connection_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Utils.h"
#include "Logger.h"
#include "Connection.h"


// class Command

	// private
	struct Command {
		Logger* logger;
		Connection* connection;
		unsigned char* buffer;
		int len;
		int chunkCount;
	};
	typedef struct Command Command;


	// public
	Command* new_Command();
	void delete_Command(Command* self);
	void Command_setLogger(Command* self,Logger* logger);
	void Command_setConnection(Command* self,Connection* connection);
	void Command_setBuffer(Command* self,unsigned char* buffer,int len);
	void Command_log(Connection* self,int level,int id,const char* message);

	// protected
	void Command_ctor(Command* self);
	void Command_dtor(Command* self);
	void Command_fatal(Command* self,int id,const char* message);
	int Connection_isHeaderOk(Connection* self);
	int Connection_scanChunks(Connection* self);
	int Connection_findChunk(Connection* self,unsigned char* sig);

#endif