#include "Command.h"
#include "Packet.h"
#include "Logger.h"


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


	void Command_setPacket(Command* self,Packet* packet) {

		self->packet = packet;
		strcpy(self->cmd,"n.a.");

	} // setPacket()


	void Command_setHashTable(Command* self,HashTable* hashTable) {
		self->hashTable = hashTable;
	} // setHashTable()


	void Command_reportStatus(Command* self,int st,int id,const char* message) {

		int logLevel;
		if (st == Command_ST_OK) {
			logLevel = Logger_NOTICE | Logger_DISPLAY;
		} else {
			logLevel = Logger_ERROR | Logger_DISPLAY;
		}

		char extendedMessage[200];
		snprintf(extendedMessage,200,"Command '%s' result: %d - %s",self->cmd,st,message);
		Packet_log(self->packet,logLevel,id,extendedMessage);

		Packet_beginChunk(self->packet,"STAT");
		Packet_appendInt(self->packet,st);
		Packet_appendStr(self->packet,message);
		Packet_endChunk(self->packet);

	} // reportStatus()


	void Command_setCommand(Command* self,unsigned char* cmd,int len) {
		if (len > 19) len = 19;
		strncpy(self->cmd,(char*)cmd,len);
	} // setCommand()


	void Command_processUnknown(Command* self) {

		Packet_prepareForReply(self->packet);
		Packet_appendHeader(self->packet);
		Packet_appendCounter(self->packet);
		Command_reportStatus(self
			,Command_ST_INVALID_COMMAND
			,2201,"Invalid command"
		);
		Packet_appendEndmark(self->packet);

	} // processUnknownCommand()


	void Command_processInfo(Command* self) {

		Packet_prepareForReply(self->packet);
		Packet_appendHeader(self->packet);
		Packet_appendCounter(self->packet);
		Command_reportStatus(self,Command_ST_OK,2202,"Info provided");
		Packet_beginChunk(self->packet,"INFO");

		// TODO: provide some info
		Packet_appendStr(self->packet,"important information");

		HashTable_hello(self->hashTable);

		Packet_endChunk(self->packet);		
		Packet_appendEndmark(self->packet);

	} // processInfo()


	void Command_processSet(Command* self) {
		printf("todo: set cmd \n");
	}


	void Command_processGet(Command* self) {
		printf("todo: get cmd \n");
	}

	
	void Command_processDel(Command* self) {
		printf("todo: del cmd \n");
	}

	
	void Command_processZap(Command* self) {
		printf("todo: zap cmd \n");
	}
	

	void Command_processKsearch(Command* self) {
		printf("todo: ksearch cmd \n");
	}


	void Command_processVsearch(Command* self) {
		printf("todo: vsearch cmd \n");
	}


	void Command_processSearch(Command* self) {
		printf("todo: search cmd \n");
	}


	void Command_processReorg(Command* self) {
		printf("todo: reorg cmd \n");
	}


