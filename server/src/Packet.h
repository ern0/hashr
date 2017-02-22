#ifndef Packet_h_included
#define Packet_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Utils.h"
#include "Logger.h"
#include "Command.h"
#include "Server.h"

#define TOKEN_BEAT 0
#define TOKEN_INFO 1
#define TOKEN_GET 2
#define TOKEN_SET 4
#define TOKEN_DEL 5
#define TOKEN_ZAP 6
#define TOKEN_KSEARCH 11
#define TOKEN_KCOUNT 12
#define TOKEN_VSEARCH 13
#define TOKEN_VCOUNT 14
#define TOKEN_SEARCH 15
#define TOKEN_COUNT 16
#define TOKEN_REORG 21
#define TOKEN_DUMP 22

// class Packet

	// const
	#define Packet_BUFLEN 20000
	
	// private
	struct Packet {
		Logger* logger;
		Server* server;
		int session;
		unsigned char* buffer;
		unsigned char internalBuffer[Packet_BUFLEN];
		int length;
		int counter;
		int chunkCount;
		int chunkBegin;
		Command* command;
	};
	typedef struct Packet Packet;


	// protected
	void Packet_ctor(Packet* self);
	void Packet_dtor(Packet* self);

	// public
	Packet* new_Packet();
	void delete_Packet(Packet* self);
	void Packet_setLogger(Packet* self,Logger* logger);
	void Packet_setServer(Packet* self,Server* server);
	void Packet_setSession(Packet* self,int session);
	void Packet_log(Packet* self,int level,int id,const char* message);
	void Packet_fatal(Packet* self,int id,const char* message);
	void Packet_setBuffer(Packet* self,unsigned char* buffer,int len);
	unsigned char* Packet_getBuffer(Packet* self);
	void Packet_setLength(Packet* self,int len);
	int Packet_getLength(Packet* self);
	int Packet_isHeaderOk(Packet* self);
	int Packet_scanChunks(Packet* self);
	int Packet_findChunk(Packet* self,char* sig);
	int Packet_process(Packet* self);
	void Packet_prepareForReply(Packet* self);
	void Packet_append(Packet* self,const char* data,int len);
	void Packet_appendInt(Packet* self,int value);
	void Packet_appendStr(Packet* self,const char* value);
	void Packet_appendHeader(Packet* self);
	void Packet_appendCounter(Packet* self);
	void Packet_appendEndmark(Packet* self);
	void Packet_beginChunk(Packet* self,const char* id);
	void Packet_endChunk(Packet* self);
	void Packet_appendChunk(Packet* self,const char* id,const char* value,int length);	
	void Packet_appendIntChunk(Packet* self,const char* id,int value);
	void Packet_appendStrChunk(Packet* self,const char* id,const char* value);

#endif