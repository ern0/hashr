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
#include "Search.h"

// class Packet;
struct Packet;
typedef struct Packet Packet;


#define Command_ST_INFO_PROVIDED 1
#define Command_ST_INSERTED 2
#define Command_ST_UPDATED 3
#define Command_ST_FOUND 4
#define Command_ST_NOT_FOUND 5
#define Command_ST_DELETED 6
#define Command_ST_NOT_EXISTS 7
#define Command_ST_EMPTY 8
#define Command_ST_ZAPPED 9
#define Command_ST_NO_MATCH 10
#define Command_ST_MATCH_FOUND 11
#define Command_ST_UNCHANGED 27
#define Command_ST_REORGANIZED 28

#define Command_ST_ERROR_LEVEL 30
#define Command_ST_INVALID_COMMAND 31
#define Command_ST_MISSING_PARAM 32

// class Command

	// private
	struct Command {
		Packet* packet;
		Server* server;
		int token;
		char unknown[20];
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
	void Command_setCommand(Command* self,int cmdToken);
	char* Command_getName(Command* self,int token);
	void Command_processHeartbeat(Command* self);
	void Command_processInfo(Command* self);
	void Command_processSet(Command* self);
	void Command_processGet(Command* self);
	void Command_processDel(Command* self);
	void Command_processZap(Command* self);
	void Command_processKsearch(Command* self);
	void Command_processVsearch(Command* self);
	void Command_processSearch(Command* self);
	void Command_processKcount(Command* self);
	void Command_processVcount(Command* self);
	void Command_processCount(Command* self);
	void Command_processReorg(Command* self);
	void Command_processDump(Command* self);

	// protected
	void Command_ctor(Command* self);
	void Command_dtor(Command* self);
	void Command_fail(Command* self,int id,const char* message);
	void Command_reportStatus(Command* self,int st,int id,const char* message);
	void Command_reportStatusExtra(Command* self,int st,int id,const char* message,const char* extra);
	int Command_findParamChunk(Command* self,char* id,int mandatory);
	int Command_loadStrChunk(Command* self,char* id,int mandatory);
	int Command_loadIntChunk(Command* self,char* id,int mandatory);
	void Command_beginReply(Command* self);
	void Command_endReply(Command* self);
	int Command_prepareSearch(Command* self,Search* search);
	void Command_universalSearch(Command* self,Search* search);


#endif