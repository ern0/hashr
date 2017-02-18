#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "../src/Utils.h"
	#include "../src/Logger.h"
	#include "../src/Packet.h"
}


TEST_CASE("Packet build") {

	Logger* logger = new_Logger();
	Logger_setLevel(logger,Logger_MUTE);
	Packet* packet = new_Packet();
	Packet_setLogger(packet,logger);
	Packet_setSession(packet,1);


	SECTION("packet append str") {

		char data[] = { 'A','B','C' };

		Packet_prepareForReply(packet);
		Packet_appendStr(packet,"ABC");

		char* result = (char*)Packet_getBuffer(packet);
		REQUIRE( 0 == strncmp(result,data,sizeof(data)) );

	}

	SECTION("packet append int") {

		char data[] = { 0x12,0x34,0x56,0x78 };

		Packet_prepareForReply(packet);
		Packet_appendInt(packet,0x12345678);

		char* result = (char*)Packet_getBuffer(packet);
		REQUIRE( 0 == strncmp(result,data,sizeof(data)) );

	}

	SECTION("packet build chunk") {

		char data[] = {
			'H','S','H','r',
			'X','M','P','L',0,0,0,6,
			'a','b','c','1','2','3',
			'e','n','d','m'
		};

		Packet_prepareForReply(packet);
		Packet_appendHeader(packet);
		Packet_beginChunk(packet,"XMPL");
		Packet_appendStr(packet,"abc123");
		Packet_endChunk(packet);

		char* result = (char*)Packet_getBuffer(packet);
		REQUIRE( 0 == strncmp(result,data,sizeof(data)) );

	}

	delete_Packet(packet);

} // test case
