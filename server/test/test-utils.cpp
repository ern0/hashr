#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "../src/Utils.h"
}


TEST_CASE("Utils") {


	SECTION("signature eq") {
		unsigned char sign1[] = "ABCD";
		unsigned char sign2[] = "ABCD";
		REQUIRE( Utils_isEqSigs(sign1,sign2) );	
	}
	SECTION("signature ne") {
		unsigned char sign1[] = "aBCD";
		unsigned char sign2[] = "ABCD";
		REQUIRE( !Utils_isEqSigs(sign1,sign2) );	
	}

	SECTION("integers in buffer 1") {
		unsigned char buffer[] = { 0x12,0x34,0x56,0x78 };
		REQUIRE( Utils_getBufInt(buffer) == 0x12345678);
	}
	SECTION("integers in buffer 2") {
		unsigned char buffer[] = { 0,0,0,0 };
		REQUIRE( Utils_getBufInt(buffer) == 0 );
	}
	SECTION("integers in buffer 3") {
		unsigned char buffer[] = { 0xff,0xff,0xff,0xff };
		REQUIRE( Utils_getBufInt(buffer) == -1 );
	}


} // test case
