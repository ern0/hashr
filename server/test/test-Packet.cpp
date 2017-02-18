#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "../src/Utils.h"
	#include "../src/Logger.h"
	#include "../src/Packet.h"
}


TEST_CASE("Packet") {

	Logger* logger = new_Logger();
	Logger_setLevel(logger,Logger_MUTE);
	Packet* packet = new_Packet();
	Packet_setLogger(packet,logger);
	Packet_setSession(packet,1);


	SECTION("packet header check pos") {
		unsigned char validHeader[] = { 'H','S','H','r' };
		Packet_setBuffer(packet,validHeader,-1);
		REQUIRE( Packet_isHeaderOk(packet) );
	}

	SECTION("packet header check neg") {
		unsigned char badHeader[] = { 'H','S','H', 0 };
		Packet_setBuffer(packet,badHeader,-1);
		REQUIRE( !Packet_isHeaderOk(packet) );
		REQUIRE( Packet_process(packet) == -1 );
		REQUIRE( Logger_getLastId(logger) == 2018 );
	}

	SECTION("packet chunk count 3") {

		unsigned char data[] = { 
			'H','S','H','r',
			'C','H','N','K',0,0,0,1,
			'x',
			'C','H','N','K',0,0,0,1,
			'y',
			'C','H','N','K',0,0,0,1,
			'z',
			'e','n','d','m'
		};
		
		Packet_setBuffer(packet,data,sizeof(data));
		REQUIRE( Packet_scanChunks(packet) == 3 );

	} // section

	SECTION("packet chunk count 0") {
		
		unsigned char data[] = { 
			'H','S','H','r',
			'e','n','d','m'
		};
		
		Packet_setBuffer(packet,data,sizeof(data));
		REQUIRE( Packet_scanChunks(packet) == 0 );

	} // section

	SECTION("packet chunk no endmark") {
		
		unsigned char data[] = { 
			'H','S','H','r',
			'C','H','N','K',0,0,0,1,
			'x',
			'C','H','N','K',0,0,0,1,
			'y',
			'C','H','N','K',0,0,0,1,
			'z'
		};
		Packet_setBuffer(packet,data,sizeof(data));
		
		REQUIRE( Packet_scanChunks(packet) == -1 );
		REQUIRE( Packet_process(packet) == -1 );
		REQUIRE( Logger_getLastId(logger) == 2019 );
	
	} // section

	SECTION("packet chunk wrong chunk length") {
		
		unsigned char data[] = { 
			'H','S','H','r',
			'C','H','N','K',0,0,0,1,
			'x',
			'C','H','N','K',0,0,0,2,
			'y',
			'C','H','N','K',0,0,0,1,
			'z',
			'e','n','d','m'
		};
		Packet_setBuffer(packet,data,sizeof(data));

		REQUIRE( Packet_scanChunks(packet) -1 );
		REQUIRE( Packet_process(packet) == -1 );
		REQUIRE( Logger_getLastId(logger) == 2019 );
	
	} // section

	delete_Packet(packet);

} // test case
