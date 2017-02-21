#ifndef Command_h_included
#define Command_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RET

#include "Utils.h"
#include "Logger.h"
#include "Server.h"
#include "HashTable.h"

// class Packet;
struct Packet;
typedef struct Packet Packet;


#define Command_ST_OK 0

#define Command_ST_INSERTED 1
#define Command_ST_UPDATED 2
#define Command_ST_FOUND 3
#define Command_ST_NOT_FOUND 4
#define Command_ST_DELETED 5
#define Command_ST_NOT_EXISTS 6

#define Command_ST_INVALID_COMMAND 21
#define Command_ST_MISSING_PARAM 22

// class Command

	// private
	struct Command {
		Packet* packet;
		Server* server;
		char cmd[20];
		int counter;
		HashTable* hashTable;
		char* data;
		int length;
	};
	typedef struct Command Command;


	// public
	Command* new_Command();
	void delete_Command(Command* self);
	void Command_setPacket(Command* self,Packet* packet);
	void Command_setServer(Command* self,Server* server);
	void Command_setHashTable(Command* self,HashTable* hashTable);
	void Command_processUnknown(Command* cmd);
	void Command_setCommand(Command* self,char* cmd,int len);
	void Command_processHeartbeat(Command* self);
	void Command_processInfo(Command* self);
	void Command_processSet(Command* self);
	void Command_processGet(Command* self);
	void Command_processDel(Command* self);
	void Command_processZap(Command* self);
	void Command_processKsearch(Command* self);
	void Command_processVsearch(Command* self);
	void Command_processSearch(Command* self);
	void Command_processReorg(Command* self);
	void Command_processDump(Command* self);

	// protected
	void Command_ctor(Command* self);
	void Command_dtor(Command* self);
	void Command_fail(Command* self,int id,const char* message);
	void Command_reportStatus(Command* self,int st,int id,const char* message);
	int Command_loadChunk(Command* self,char* id);
	void Command_beginReply(Command* self);
	void Command_endReply(Command* self);


#endif