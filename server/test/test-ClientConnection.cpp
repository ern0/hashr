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

	SECTION("packet header check") {

		unsigned char validHeader[] = { 'H','S','H','r' };
		ClientConnection_setBuffer(conn,validHeader);
		REQUIRE( ClientConnection_isHeaderOk(conn) );

		unsigned char badHeader[] = { 'H','S','H', 0 };
		ClientConnection_setBuffer(conn,badHeader);
		REQUIRE( !ClientConnection_isHeaderOk(conn) );

	}


} // test case
