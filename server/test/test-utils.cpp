#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "../src/Utils.h"
}


TEST_CASE("Utils") {


	SECTION("signature eq") {
		char sign1[] = "ABCD";
		char sign2[] = "ABCD";
		REQUIRE( Utils_isEqSigs(sign1,sign2) );	
	}
	SECTION("signature ne") {
		char sign1[] = "aBCD";
		char sign2[] = "ABCD";
		REQUIRE( !Utils_isEqSigs(sign1,sign2) );	
	}


	SECTION("read int from buffer 1") {
		unsigned char buffer[] = { 0x12,0x34,0x56,0x99 };
		REQUIRE( Utils_getBufInt(buffer) == 0x12345699);
	}
	SECTION("read int from buffer 2") {
		unsigned char buffer[] = { 0,0,0,0 };
		REQUIRE( Utils_getBufInt(buffer) == 0 );
	}
	SECTION("read int from buffer 3") {
		unsigned char buffer[] = { 0xff,0xff,0xff,0xff };
		REQUIRE( Utils_getBufInt(buffer) == -1 );
	}


	SECTION("write int to buffer 1") {
		unsigned char buffer[5];
		for (int i = 0; i < 5; i++) buffer[i] = 0xff;
		Utils_setBufInt(buffer,8);
		REQUIRE(buffer[0] == 0);
		REQUIRE(buffer[1] == 0);
		REQUIRE(buffer[2] == 0);
		REQUIRE(buffer[3] == 8);
		REQUIRE(buffer[4] == 0xff);
	}

	SECTION("write int to buffer 2") {
		unsigned char buffer[5];
		for (int i = 0; i < 5; i++) buffer[i] = 0xff;
		Utils_setBufInt(buffer,0x12345699);
		REQUIRE(buffer[0] == 0x12);
		REQUIRE(buffer[1] == 0x34);
		REQUIRE(buffer[2] == 0x56);
		REQUIRE(buffer[3] == 0x99);
		REQUIRE(buffer[4] == 0xff);
	}

	SECTION("write int to buffer 3") {
		unsigned char buffer[5];
		for (int i = 0; i < 5; i++) buffer[i] = 0;
		Utils_setBufInt(buffer,-1);
		REQUIRE(buffer[0] == 0xff);
		REQUIRE(buffer[1] == 0xff);
		REQUIRE(buffer[2] == 0xff);
		REQUIRE(buffer[3] == 0xff);
		REQUIRE(buffer[4] == 0);
	}

	SECTION("round up to power of 2") {

		REQUIRE( Utils_roundUp2Power(3) == 4 );

		REQUIRE( Utils_roundUp2Power(12) == 16 );
		REQUIRE( Utils_roundUp2Power(15) == 16 );
		REQUIRE( Utils_roundUp2Power(16) == 16 );
		REQUIRE( Utils_roundUp2Power(17) == 32 );

		REQUIRE( Utils_roundUp2Power(65000) == 65536 );
		REQUIRE( Utils_roundUp2Power(65536) == 65536 );

	}

	SECTION("calculate hash masks") {

		REQUIRE( Utils_calcHashMask(3) == 0x03 );

		REQUIRE( Utils_calcHashMask(7) == 0x07 );
		REQUIRE( Utils_calcHashMask(8) == 0x07 );
		REQUIRE( Utils_calcHashMask(9) == 0x0f );

		REQUIRE( Utils_calcHashMask(65535) == 0xffff );
		REQUIRE( Utils_calcHashMask(65536) == 0xffff );
		REQUIRE( Utils_calcHashMask(65537) == 0x1ffff );

	}

} // test case
