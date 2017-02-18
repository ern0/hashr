#ifndef Packet_h_included
#define Packet_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Utils.h"
#include "Logger.h"


// class Packet

	// const
	#define Packet_BUFLEN 20000
	
	// private
	struct Packet {
		Logger* logger;
		int session;
		unsigned char* buffer;
		unsigned char internalBuffer[Packet_BUFLEN];
		int len;
		int chunkCount;
	};
	typedef struct Packet Packet;



	// public
	Packet* new_Packet();
	void delete_Packet(Packet* self);
	void Packet_setLogger(Packet* self,Logger* logger);
	void Packet_setSession(Packet* self,int session);
	void Packet_log(Packet* self,int level,int id,const char* message);
	void Packet_fatal(Packet* self,int id,const char* message);
	void Packet_setBuffer(Packet* self,unsigned char* buffer,int len);
	unsigned char* Packet_getBuffer(Packet* self);
	void Packet_setLength(Packet* self,int len);
	int Packet_getLength(Packet* self);
	int Packet_isHeaderOk(Packet* self);
	int Packet_scanChunks(Packet* self);
	int Packet_findChunk(Packet* self,unsigned char* sig);
	int Packet_process(Packet* self);

	// protected
	void Packet_ctor(Packet* self);
	void Packet_dtor(Packet* self);

#endif