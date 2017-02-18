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
		self->chunkCount = -1;
		Packet_setBuffer(self,self->internalBuffer,-1);

	} // ctor


	void Packet_dtor(Packet* self) {

		// NOP

	} // dtor


	void Packet_setLogger(Packet* self,Logger* logger) {
		self->logger = logger;
	} // setLogger()


	void Packet_setSession(Packet* self,int session) {
		self->session = session;
	} // setSession()


	void Packet_setBuffer(Packet* self,unsigned char* buffer,int len) {
		self->buffer = buffer;
		self->len = len;
	} // setBuffer()


	unsigned char* Packet_getBuffer(Packet* self) {
		return self->buffer;
	} // getBuffer()


	void Packet_setLength(Packet* self,int len) {
		self->len = len;
	} // setLength()


	int Packet_getLength(Packet* self) {
		return self->len;
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

			if (self->len < index + 4) return -1;
			
			if (Utils_isEqSigs(&self->buffer[index],(const unsigned char*)"endm")) {
				break;
			}

			int chunkLength = Utils_getBufInt(&self->buffer[index + 4]);
			if (chunkLength < 0) return -1;

			result++;
			index += chunkLength + 8;
					
		} // loop

		return result;
	} // scanChunks()


	int Packet_findChunk(Packet* self,unsigned char* sig) {

		int index = 4;

		while (1) {

			if (Utils_isEqSigs(&self->buffer[index],(const unsigned char*)"endm")) {
				return -1;
			}

			if (Utils_isEqSigs(&self->buffer[index],sig)) break;

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

		printf("todo: proc \n");
	
		return 0;
	} // process()


	