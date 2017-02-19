#include "Command.h"
#include "Server.h"
#include "Packet.h"


	Packet* new_Packet() {

		Packet* self;
		self = (Packet*)malloc(sizeof(Packet));
		if (self != NULL) Packet_ctor(self);

		return self;
	} // new 


	void delete_Packet(Packet* self) {
		Packet_dtor(self);
		free(self);
	} // delete 


	void Packet_ctor(Packet* self) {

		self->session = -1;
		self->counter = -1;
		self->chunkCount = -1;
		Packet_setBuffer(self,self->internalBuffer,-1);

		self->command = new_Command();

	} // ctor


	void Packet_dtor(Packet* self) {
		delete_Command(self->command);
	} // dtor


	void Packet_setLogger(Packet* self,Logger* logger) {
		self->logger = logger;
	} // setLogger()


	void Packet_setServer(Packet* self,Server* server) {

		self->server = server;

		Command_setServer(self->command,self->server);

		HashTable* hashTable = Server_getHashTable(self->server);
		Command_setHashTable(self->command,hashTable);

	} // setServer()


	void Packet_setSession(Packet* self,int session) {
		self->session = session;
	} // setSession()


	void Packet_setBuffer(Packet* self,unsigned char* buffer,int len) {
		self->buffer = buffer;
		self->length = len;
	} // setBuffer()


	unsigned char* Packet_getBuffer(Packet* self) {
		return self->buffer;
	} // getBuffer()


	void Packet_setLength(Packet* self,int len) {
		self->length = len;
	} // setLength()


	int Packet_getLength(Packet* self) {
		return self->length;
	} // getLength()


	void Packet_log(Packet* self,int level,int id,const char* message) {

		char extendedMessage[200];
		strncpy(extendedMessage,message,199);
		strncat(extendedMessage," (session=%d)",199);

		Logger_logi(self->logger,level,id,extendedMessage,self->session);

	} // log()


	void Packet_fatal(Packet* self,int id,const char* message) {
		Packet_log(self,Logger_FATAL,id,message);
		exit(2);
	} // fatal()


	int Packet_isHeaderOk(Packet* self) {

		if ( self->buffer[0] != 'H' ) return 0;
		if ( self->buffer[1] != 'S' ) return 0;
		if ( self->buffer[2] != 'H' ) return 0;
		if ( self->buffer[3] != 'r' ) return 0;

		return 1;
	} // isHeaderOk()


	int Packet_scanChunks(Packet* self) {

		int result = 0;
		int index = 4;

		while (1) {

			if (self->length < index + 4) return -1;
			
			if (Utils_isEqSigs((char*)&self->buffer[index],"endm")) {
				break;
			}

			int chunkLength = Utils_getBufInt(&self->buffer[index + 4]);
			if (chunkLength < 0) return -1;

			result++;
			index += chunkLength + 8;
					
		} // loop

		return result;
	} // scanChunks()


	int Packet_findChunk(Packet* self,char* sig) {

		int index = 4;

		while (1) {

			if (Utils_isEqSigs((char*)&self->buffer[index],"endm")) {
				return -1;
			}

			if (Utils_isEqSigs((char*)&self->buffer[index],sig)) break;

			int chunkLength = Utils_getBufInt(&self->buffer[index + 4]);
			index += chunkLength + 8;
					
		} // loop

		return index + 8;
	} // findChunks()


	int Packet_process(Packet* self) {

		if (!Packet_isHeaderOk(self)) {
			Packet_log(self,Logger_ERROR | Logger_DISPLAY,2018,"Invalid request");
			return -1;		
		} // if invalid request

		self->chunkCount = Packet_scanChunks(self);
		if (self->chunkCount == -1) {
			Packet_log(self,Logger_ERROR | Logger_DISPLAY,2019,"Damaged request");
			return -1;		
		} // if damaged request

		int cmdIndex = Packet_findChunk(self,"CMND");
		if (cmdIndex == -1) {
			Packet_log(self,Logger_ERROR | Logger_DISPLAY,2020,"No command in request");
			return -1;
		} // if no command in packet

		char* cmdBuffer = (char*)&self->buffer[cmdIndex];
		int cmdLen = Utils_getBufInt(&self->buffer[cmdIndex - 4]);

		int counterIndex = Packet_findChunk(self,"cntr");
		if (counterIndex != -1) {
			self->counter = Utils_getBufInt(&self->buffer[counterIndex]);
		}	else {
			self->counter = -1;
		}	

		Command_setPacket(self->command,self);
		Command_setCommand(self->command,cmdBuffer,cmdLen);

		if (0 == strncmp("beat",cmdBuffer,cmdLen)) {
			Command_processHeartbeat(self->command);
		} else if (0 == strncmp("get",cmdBuffer,cmdLen)) {
			Command_processGet(self->command);
		} else if (0 == strncmp("set",cmdBuffer,cmdLen)) {
			Command_processSet(self->command);
		} else if (0 == strncmp("del",cmdBuffer,cmdLen)) {
			Command_processDel(self->command);
		} else if (0 == strncmp("info",cmdBuffer,cmdLen)) {
			Command_processInfo(self->command);
		} else if (0 == strncmp("zap",cmdBuffer,cmdLen)) {
			Command_processZap(self->command);
		} else if (0 == strncmp("ksearch",cmdBuffer,cmdLen)) {
			Command_processKsearch(self->command);
		} else if (0 == strncmp("vsearch",cmdBuffer,cmdLen)) {
			Command_processVsearch(self->command);
		} else if (0 == strncmp("search",cmdBuffer,cmdLen)) {
			Command_processSearch(self->command);
		} else if (0 == strncmp("reorg",cmdBuffer,cmdLen)) {
			Command_processReorg(self->command);
		} else {
			Command_processUnknown(self->command);
		}
	
		return 0;
	} // process()


	void Packet_prepareForReply(Packet* self) {
		Packet_setBuffer(self,self->internalBuffer,0);
	} // prepareForReply()


	void Packet_append(Packet* self,const char* data,int len) {
		memcpy(&self->buffer[self->length],data,len);
		self->length += len;
	} // append()


	void Packet_appendInt(Packet* self,int value) {
		
		unsigned char buf[4];
		Utils_setBufInt(buf,value);

		Packet_append(self,(char*)buf,4);

	} // appendInt()


	void Packet_appendStr(Packet* self,const char* value) {
		Packet_append(self,value,strlen(value));
	} // appendStr()


	void Packet_appendHeader(Packet* self) {
		Packet_append(self,"HSHr",4);
	} // appendHeader()


	void Packet_appendCounter(Packet* self) {
		if (self->counter == -1) return;
		Packet_appendIntChunk(self,"cntr",self->counter);
	} // appendCounter()


	void Packet_appendEndmark(Packet* self) {
		Packet_append(self,"endm",4);
	} // appendHeader()


	void Packet_beginChunk(Packet* self,const char* id) {
		Packet_appendStr(self,id);
		self->chunkBegin = self->length;
		self->length += 4;
	} // beginChunk()


	void Packet_endChunk(Packet* self) {

		int tmp = self->length;
		self->length = self->chunkBegin;
		Packet_appendInt(self,tmp - self->length - 4);
		self->length = tmp;

	} // endChunk()


	void Packet_appendIntChunk(Packet* self,const char* id,int value) {

		Packet_beginChunk(self,id);
		Packet_appendInt(self,value);
		Packet_endChunk(self);
 
	} // appendIntChunk()


	void Packet_appendStrChunk(Packet* self,const char* id,const char* value) {

		Packet_beginChunk(self,id);
		Packet_appendStr(self,value);
		Packet_endChunk(self);

	} // appendStrChunk()
