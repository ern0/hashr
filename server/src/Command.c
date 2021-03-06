#include "Command.h"
#include "Packet.h"
#include "Logger.h"
#include "Server.h"
#include "HashItem.h"


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
		self->token = -1;
	} // setPacket()


	void Command_setHashTable(Command* self,HashTable* hashTable) {
		self->hashTable = hashTable;
	} // setHashTable()


	void Command_reportStatus(Command* self,int st,int id,const char* message) {
		Command_reportStatusExtra(self,st,id,message,"");
	} // reportStatus()


	void Command_reportStatusExtra(Command* self,int st,int id,const char* message,const char* extra) {

		int logLevel;
		if (st < Command_ST_ERROR_LEVEL) {
			logLevel = Logger_NOTICE | Logger_DISPLAY;
		} else {
			logLevel = Logger_ERROR | Logger_DISPLAY;
		}

		char extendedMessage[200];
		snprintf(
			extendedMessage
			,200
			,"Command \"%s\" result: %d - %s%s%s"
			,Command_getName(self,self->token)
			,st
			,message
			,( extra[0] != 0 ? ": " : "")
			,extra
		);
		Packet_log(self->packet,logLevel,id,extendedMessage);

		Packet_beginChunk(self->packet,"STAT");
		Packet_appendInt(self->packet,st);
		Packet_appendStr(self->packet,message);
		Packet_endChunk(self->packet);

	} // reportStatusExtra()


	void Command_setCommand(Command* self,int token) {
		self->token = token;
	} // setCommand()


	char* Command_getName(Command* self,int token) {

		switch (token) {
			case TOKEN_INFO:
				return (char*)"info"; 
			case TOKEN_GET:
				return (char*)"get";
			case TOKEN_SET:
				return (char*)"set";
			case TOKEN_DEL:
				return (char*)"del";
			case TOKEN_ZAP:
				return (char*)"zap"; 
			case TOKEN_KSEARCH:
				return (char*)"ksearch";
			case TOKEN_KCOUNT:
				return (char*)"kcount";
			case TOKEN_VSEARCH:
				return (char*)"vsearch";
			case TOKEN_VCOUNT:
				return (char*)"vcount";
			case TOKEN_SEARCH:
				return (char*)"search";
			case TOKEN_COUNT:
				return (char*)"count";
			case TOKEN_REORG:
				return (char*)"reorg";
			case TOKEN_DUMP:
				return (char*)"dump";
		} // switch

		sprintf(self->unknown,"unknown-%d",token);
		return self->unknown;
	} // getName()


	int Command_findParamChunk(Command* self,char* id,int mandatory) {

		int chunkIndex = Packet_findChunk(self->packet,id);
		if (!mandatory) return chunkIndex;

		if (chunkIndex == -1) {

			Command_beginReply(self);

			Command_reportStatusExtra(
				self
				,Command_ST_MISSING_PARAM
				,2203,"missing parameter"
				,id
			);

			Command_endReply(self);

			return -1;
		} // if not found

		return chunkIndex;
	} // findParamChunk()


	int Command_loadStrChunk(Command* self,char* id,int mandatory) {

		int chunkIndex = Command_findParamChunk(self,id,mandatory);
		if (chunkIndex == -1) return -1;

		unsigned char* buffer = Packet_getBuffer(self->packet);
		self->data = (char*)&buffer[chunkIndex];
		self->length = Utils_getBufInt(&buffer[chunkIndex - 4]);

		return 0;
	} // loadStrCunk()


	int Command_loadIntChunk(Command* self,char* id,int mandatory) {

		int chunkIndex = Command_findParamChunk(self,id,mandatory);
		if (chunkIndex == -1) return -1;

		unsigned char* buffer = Packet_getBuffer(self->packet);
		int length = Utils_getBufInt(&buffer[chunkIndex - 4]);
		if (length < 4) return -1;
		int result = Utils_getBufInt(&buffer[chunkIndex]);

		return result;
	} // loadIntChunk()


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
			,2201,"invalid command"
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

		Command_reportStatus(self,Command_ST_INFO_PROVIDED,2202,"info provided");
	
		int noOfConns = Server_getNumberOfConnections(self->server);
		Packet_appendIntChunk(self->packet,"NCON",noOfConns);

		int method = HashTable_getMethod(self->hashTable);
		Packet_appendIntChunk(self->packet,"METD",method);

		int minCapacity = HashTable_getMinCapacity(self->hashTable);
		Packet_appendIntChunk(self->packet,"MCAP",minCapacity);

		int capacity = HashTable_getCapacity(self->hashTable);
		Packet_appendIntChunk(self->packet,"CPTY",capacity);
		
		int noOfElms = HashTable_getNumberOfElms(self->hashTable);
		Packet_appendIntChunk(self->packet,"NELM",noOfElms);

		int collision = HashTable_getCollisionPercent(self->hashTable);
		Packet_appendIntChunk(self->packet,"COLP",collision);

		Command_endReply(self);

	} // processInfo()


	void Command_processSet(Command* self) {

		if ( Command_loadStrChunk(self,"QKEY",1) == -1 ) return;
		char* keyData = self->data;
		int keyLength = self->length;

		if ( Command_loadStrChunk(self,"QVAL",1) == -1 ) return;
		char* valueData = self->data;
		int valueLength = self->length;

		Command_beginReply(self);

		int oldCapacity = HashTable_getCapacity(self->hashTable);

		int result = HashTable_performSet(
			self->hashTable
			,keyData,keyLength
			,valueData,valueLength
		);

		if (result == HashTable_SET_UPDATED) {
			Command_reportStatus(
				self
				,Command_ST_UPDATED
				,2210,"data updated"
			);
		} // if inserted

		else {
			Command_reportStatus(
				self
				,Command_ST_INSERTED
				,2211,"data inserted"
			);
		} // else updated

		int capacity = HashTable_getCapacity(self->hashTable);
		if (oldCapacity != capacity) {
			Packet_appendIntChunk(self->packet,"CPTY",capacity);
		}

		Command_endReply(self);

	} // processSet()


	void Command_processGet(Command* self) {

		if ( Command_loadStrChunk(self,"QKEY",1) == -1 ) return;

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

		if (result == HashTable_GET_PROVIDED) {
			Command_reportStatus(
				self
				,Command_ST_FOUND
				,2212,"data found"
			);

			Packet_appendChunk(self->packet,"AVAL",valueData,valueLength);

		} // if found

		else {
			Command_reportStatus(
				self
				,Command_ST_NOT_FOUND
				,2213,"no such key"
			);
		} // else not found

		Command_endReply(self);

	} // get()

	
	void Command_processDel(Command* self) {

		if ( Command_loadStrChunk(self,"QKEY",1) == -1 ) return;
		char* keyData = self->data;
		int keyLength = self->length;

		Command_beginReply(self);

		int oldCapacity = HashTable_getCapacity(self->hashTable);

		char* valueData = NULL;
		int valueLength = 0;
		int result = HashTable_performDel(
			self->hashTable
			,keyData,keyLength
		);

		if (result == HashTable_DEL_DELETED) {
			Command_reportStatus(
				self
				,Command_ST_DELETED
				,2214,"data deleted"
			);
		} // if found

		else {
			Command_reportStatus(
				self
				,Command_ST_NOT_EXISTS
				,2215,"Key not exists"
			);
		} // else not found

		int capacity = HashTable_getCapacity(self->hashTable);
		if (oldCapacity != capacity) {
			Packet_appendIntChunk(self->packet,"CPTY",capacity);
		}

		Command_endReply(self);

	} // processDel()

	
	void Command_processZap(Command* self) {

		Command_beginReply(self);

		int zapped = HashTable_getNumberOfElms(self->hashTable);
		int oldCapacity = HashTable_getCapacity(self->hashTable);

		if (HashTable_performZap(self->hashTable) == HashTable_ZAP_EMPTY) {
			Command_reportStatus(
				self
				,Command_ST_EMPTY
				,2216,"already empty"
			);
		} // if already empty

		else {
			Command_reportStatus(
				self
				,Command_ST_ZAPPED
				,2217,"all items deleted"
			);
		} // else zapped

		Packet_appendIntChunk(self->packet,"ZAPD",zapped);

		int capacity = HashTable_getCapacity(self->hashTable);
		if (oldCapacity != capacity) {
			Packet_appendIntChunk(self->packet,"CPTY",capacity);
		}

		Command_endReply(self);

	} // processZap()
	

	void Command_processKcount(Command* self) {
		
		Search* search = new_Search();
		if (search == NULL) return;

		Search_setCountMode(search,SEARCH_MATCH_KEY);
		int seo = Command_prepareSearch(self,search);
		if (seo != -1) Command_universalSearch(self,search);

		delete_Search(search);

	} // processKcount()


	void Command_processVcount(Command* self) {
		
		Search* search = new_Search();
		if (search == NULL) return;

		Search_setCountMode(search,SEARCH_MATCH_VALUE);
		int seo = Command_prepareSearch(self,search);
		if (seo != -1) Command_universalSearch(self,search);

		delete_Search(search);

	} // processVcount()


	void Command_processCount(Command* self) {

		Search* search = new_Search();
		if (search == NULL) return;

		Search_setCountMode(search,SEARCH_MATCH_KEY | SEARCH_MATCH_VALUE);
		int seo = Command_prepareSearch(self,search);
		if (seo != -1) Command_universalSearch(self,search);

		delete_Search(search);

	} // processCount()


	void Command_processKsearch(Command* self) {
		
		Search* search = new_Search();
		if (search == NULL) return;

		Search_setSearchMode(search,SEARCH_MATCH_KEY);
		int seo = Command_prepareSearch(self,search);
		if (seo != -1) Command_universalSearch(self,search);

		delete_Search(search);

	} // processKsearch()


	void Command_processVsearch(Command* self) {
		
		Search* search = new_Search();
		if (search == NULL) return;

		Search_setSearchMode(search,SEARCH_MATCH_VALUE);
		int seo = Command_prepareSearch(self,search);
		if (seo != -1) Command_universalSearch(self,search);

		delete_Search(search);

	} // processVsearch()


	void Command_processSearch(Command* self) {
		
		Search* search = new_Search();
		if (search == NULL) return;

		Search_setSearchMode(search,SEARCH_MATCH_KEY | SEARCH_MATCH_VALUE);
		int seo = Command_prepareSearch(self,search);
		if (seo != -1) Command_universalSearch(self,search);

		delete_Search(search);

	} // processSearch()


	int Command_prepareSearch(Command* self,Search* search) {

		if (Search_isSearchMode(search)) {

			int limitStart = Command_loadIntChunk(self,"LIMS",0);
			if (limitStart == -1) limitStart = 0;
			Search_setLimitStart(search,limitStart);

			int limitItems = Command_loadIntChunk(self,"LIMI",0);
			if (limitItems < 1) limitItems = -1;
			Search_setLimitItems(search,limitItems);

		} // if search mode

		if ( Command_loadStrChunk(self,"SPAT",0) == -1 ) {

			Command_beginReply(self);			
			Command_reportStatus(
				self
				,Command_ST_MISSING_PARAM
				,2218,"missing search pattern"
			);
			Command_endReply(self);

			return -1;
		} // if missing pattern

		Search_setPattern(search,self->data,self->length);

		return 0;
	} // prepareSearch()


	void Command_universalSearch(Command* self,Search* search) {

		Command_beginReply(self);

		int result = HashTable_performSearch(self->hashTable,search);

		if (result == HashTable_SEARCH_NOT_FOUND) {
			Command_reportStatus(
				self
				,Command_ST_NO_MATCH
				,2219,"no match"
			);
		} // if empty

		else {
			Command_reportStatus(
				self
				,Command_ST_MATCH_FOUND
				,2220,"search result provided"
			);
		} // if any hits

		Packet_appendIntChunk(self->packet,"SRES",search->numberOfResults);

		int provideResult = 1;
		if (Search_isCountMode(search)) provideResult = 0;
		if (Search_getNumberOfResults(search) == 0) provideResult = 0;
		if (provideResult) {

			int num = Search_getNumberOfResults(search);
			for (int i = 0; i < num; i++) {

				HashItem* item = Search_getResult(search,i);

				char* data = HashItem_getKeyData(item);
				int length = HashItem_getKeyLength(item);
				Packet_appendChunk(self->packet,"AKEY",data,length);

				data = HashItem_getValueData(item);
				length = HashItem_getValueLength(item);
				Packet_appendChunk(self->packet,"AVAL",data,length);

			} // foreach results

		} // if search

		Command_endReply(self);
		
	} // universalSearch()


	void Command_processReorg(Command* self) {

		Command_beginReply(self);

		int method = Command_loadIntChunk(self,"RMET",1);
		if (method == -1) return;

		int minCapacity = Command_loadIntChunk(self,"RCAP",0);
		HashTable_calcMinCapacity(self->hashTable,minCapacity);

		int result = HashTable_performReorg(self->hashTable,method);

		if (result == HashTable_REORG_UNCHANGED) {
			Command_reportStatus(
				self
				,Command_ST_UNCHANGED
				,2221,"unchanged parameters"
			);
		} // if unchanged

		else {
			Command_reportStatus(
				self
				,Command_ST_REORGANIZED
				,2222,"hashtable reorganized"
			);
		}

		Packet_appendIntChunk(self->packet,"METD",HashTable_getMethod(self->hashTable));
		Packet_appendIntChunk(self->packet,"CPTY",HashTable_getCapacity(self->hashTable));

		Command_endReply(self);

	} // processReorg()
