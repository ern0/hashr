#include "Command.h"
#include "Packet.h"
#include "Logger.h"
#include "Server.h"


	Command* new_Command() {

		Command* self;
		self = (Command*)malloc(sizeof(Command));
		if (self != NULL) Command_ctor(self);

		return self;
	} // new 


	void delete_Command(Command* self) {
		Command_dtor(self);
		free(self);
	} // delete 


	void Command_ctor(Command* self) {
		// NOP
	} // ctor


	void Command_dtor(Command* self) {
		// NOP
	} // dtor


	void Command_setServer(Command* self,Server* server) {
		self->server = server;
	} // setServer()


	void Command_setPacket(Command* self,Packet* packet) {
		self->packet = packet;
		strcpy(self->cmd,"n.a.");
	} // setPacket()


	void Command_setHashTable(Command* self,HashTable* hashTable) {
		self->hashTable = hashTable;
	} // setHashTable()


	void Command_reportStatus(Command* self,int st,int id,const char* message) {

		int logLevel;
		if (st < 20) {
			logLevel = Logger_NOTICE | Logger_DISPLAY;
		} else {
			logLevel = Logger_ERROR | Logger_DISPLAY;
		}

		char extendedMessage[200];
		snprintf(
			extendedMessage
			,200
			,"Command \"%s\" result: %d - %s"
			,self->cmd
			,st
			,message
		);
		Packet_log(self->packet,logLevel,id,extendedMessage);

		Packet_beginChunk(self->packet,"STAT");
		Packet_appendInt(self->packet,st);
		Packet_appendStr(self->packet,message);
		Packet_endChunk(self->packet);

	} // reportStatus()


	void Command_setCommand(Command* self,char* cmd,int len) {

		if (len > 19) len = 19;
		strncpy(self->cmd,(char*)cmd,len);

		self->cmd[len] = 0;

	} // setCommand()


	int Command_loadChunk(Command* self,char* id) {

		int chunkIndex = Packet_findChunk(self->packet,id);
		if (chunkIndex == -1) {

			Command_beginReply(self);

			Command_reportStatus(
				self
				,Command_ST_MISSING_PARAM
				,2203,"Missing parameter"
			);

			Command_endReply(self);

			return -1;
		} // if not found

		unsigned char* buffer = Packet_getBuffer(self->packet);
		self->data = (char*)&buffer[chunkIndex];
		self->length = Utils_getBufInt(&buffer[chunkIndex - 4]);

		return 0;
	} // loadCunk()


	void Command_beginReply(Command* self) {

		Packet_prepareForReply(self->packet);
		Packet_appendHeader(self->packet);
		Packet_appendCounter(self->packet);

	} // beginReply()


	void Command_endReply(Command* self) {

		Packet_appendEndmark(self->packet);

	} // endReply()


	void Command_processUnknown(Command* self) {

		Command_beginReply(self);

		Command_reportStatus(
			self
			,Command_ST_INVALID_COMMAND
			,2201,"Invalid command"
		);

		Command_endReply(self);

	} // processUnknownCommand()


	void Command_processHeartbeat(Command* self) {

		Command_beginReply(self);
		Packet_appendStrChunk(self->packet,"beat","heartbeat");
		Command_endReply(self);

	} // processHeartbeat()


	void Command_processDump(Command* self) {

		Command_beginReply(self);
		HashTable_dump(self->hashTable);
		Command_endReply(self);
		
	} // processDump()


	void Command_processInfo(Command* self) {

		Command_beginReply(self);

		Command_reportStatus(self,Command_ST_OK,2202,"Info provided");
	
		int noOfConns = Server_getNumberOfConnections(self->server);
		Packet_appendIntChunk(self->packet,"NCON",noOfConns);

		int method = HashTable_getMethod(self->hashTable);
		Packet_appendIntChunk(self->packet,"METD",method);

		int capacity = HashTable_getCapacity(self->hashTable);
		Packet_appendIntChunk(self->packet,"CPTY",capacity);
		
		int noOfElms = HashTable_getNumberOfElms(self->hashTable);
		Packet_appendIntChunk(self->packet,"NELM",noOfElms);

		Command_endReply(self);

	} // processInfo()


	void Command_processSet(Command* self) {

		if ( Command_loadChunk(self,"QKEY") == -1 ) return;
		char* keyData = self->data;
		int keyLength = self->length;

		if ( Command_loadChunk(self,"QVAL") == -1 ) return;
		char* valueData = self->data;
		int valueLength = self->length;

		Command_beginReply(self);

		int result = HashTable_performSet(
			self->hashTable
			,keyData,keyLength
			,valueData,valueLength
		);

		Command_beginReply(self);

		if (result == 1) {
			Command_reportStatus(
				self
				,Command_ST_UPDATED
				,2210,"Data updated"
			);
		} // if inserted

		else {
			Command_reportStatus(
				self
				,Command_ST_INSERTED
				,2211,"Data inserted"
			);
		} // else updated

		Command_endReply(self);

	} // processSet()


	void Command_processGet(Command* self) {

		if ( Command_loadChunk(self,"QKEY") == -1 ) return;

		Command_beginReply(self);

		char* keyData = self->data;
		int keyLength = self->length;

		char* valueData = NULL;
		int valueLength = 0;
		int result = HashTable_performGet(
			self->hashTable
			,RET &valueData,RET &valueLength
			,keyData,keyLength
		);

		if (result == 1) {
			Command_reportStatus(
				self
				,Command_ST_FOUND
				,2212,"Data found"
			);

			Packet_appendChunk(self->packet,"AVAL",valueData,valueLength);

		} // if found

		else {
			Command_reportStatus(
				self
				,Command_ST_NOT_FOUND
				,2213,"No such key"
			);
		} // else not found

		Command_endReply(self);

	} // get()

	
	void Command_processDel(Command* self) {

		if ( Command_loadChunk(self,"QKEY") == -1 ) return;
		char* keyData = self->data;
		int keyLength = self->length;

		Command_beginReply(self);

		char* valueData = NULL;
		int valueLength = 0;
		int result = HashTable_performDel(
			self->hashTable
			,keyData,keyLength
		);

		if (result == 1) {
			Command_reportStatus(
				self
				,Command_ST_DELETED
				,2214,"Data deleted"
			);
		} // if found

		else {
			Command_reportStatus(
				self
				,Command_ST_NOT_EXISTS
				,2215,"Key not exists"
			);
		} // else not found

		Command_endReply(self);

	} // processDel()

	
	void Command_processZap(Command* self) {

		Command_beginReply(self);

		int zapped = HashTable_getNumberOfElms(self->hashTable);

		if (HashTable_performZap(self->hashTable) == 1) {
			Command_reportStatus(
				self
				,Command_ST_DELETED
				,2216,"Already empty"
			);
		} // if already empty

		else {
			Command_reportStatus(
				self
				,Command_ST_NOT_EXISTS
				,2217,"All items deleted"
			);
		} // else zapped

		Packet_appendIntChunk(self->packet,"ZAPD",zapped);

		Command_endReply(self);

	} // processZap()
	

	void Command_processKsearch(Command* self) {
		Command_beginReply(self);
		printf("todo: ksearch cmd \n");
		Command_endReply(self);
	}


	void Command_processVsearch(Command* self) {
		Command_beginReply(self);
		printf("todo: vsearch cmd \n");
		Command_endReply(self);
	}


	void Command_processSearch(Command* self) {
		Command_beginReply(self);
		printf("todo: search cmd \n");
		Command_endReply(self);
	}


	void Command_processReorg(Command* self) {
		Command_beginReply(self);
		printf("todo: reorg cmd \n");
		Command_endReply(self);
	}

