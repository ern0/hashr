#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "../src/Utils.h"
	#include "../src/Logger.h"
	#include "../src/ClientConnection.h"
}


TEST_CASE("ClientConnection") {

	Logger* logger = new_Logger();
	Logger_setLevel(logger,Logger_MUTE);
	ClientConnection* conn = new_ClientConnection();
	ClientConnection_setLogger(conn,logger);


	SECTION("packet header check pos") {
		unsigned char validHeader[] = { 'H','S','H','r' };
		ClientConnection_setBuffer(conn,validHeader,-1);
		REQUIRE( ClientConnection_isHeaderOk(conn) );
	}

	SECTION("packet header check neg") {
		unsigned char badHeader[] = { 'H','S','H', 0 };
		ClientConnection_setBuffer(conn,badHeader,-1);
		REQUIRE( !ClientConnection_isHeaderOk(conn) );
		REQUIRE( ClientConnection_processPacket(conn) == -1 );
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
		
		ClientConnection_setBuffer(conn,data,sizeof(data));
		REQUIRE( ClientConnection_scanChunks(conn) == 3 );

	} // section

	SECTION("packet chunk count 0") {
		
		unsigned char data[] = { 
			'H','S','H','r',
			'e','n','d','m'
		};
		
		ClientConnection_setBuffer(conn,data,sizeof(data));
		REQUIRE( ClientConnection_scanChunks(conn) == 0 );

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
		ClientConnection_setBuffer(conn,data,sizeof(data));
		
		REQUIRE( ClientConnection_scanChunks(conn) == -1 );
		REQUIRE( ClientConnection_processPacket(conn) == -1 );
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
		ClientConnection_setBuffer(conn,data,sizeof(data));

		REQUIRE( ClientConnection_scanChunks(conn) -1 );
		REQUIRE( ClientConnection_processPacket(conn) == -1 );
		REQUIRE( Logger_getLastId(logger) == 2019 );
	
	} // section

	delete_ClientConnection(conn);

} // test case
