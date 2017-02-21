#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "../src/Utils.h"
	#include "../src/Logger.h"
	#include "../src/HashTable.h"
	#include "../src/Hasher.h"
}


// CAUTION: In some test you may notice that test data is 
// given as constant literal (casted to char*). In these cases
// the lenght of the data contains the string closing zero.
// This makes possible to use strcmp() against a constant
// literal (which is also casted to char*) for checking result.
// One may say, memmem() may be used for this, but strcmp() is
// simpler, has less parameters, less to type, less errors.

// CAUTION: These tests are using the "debug" hash method.
// It takes the first character, and repeats it in higher
// bytes of the hash longword (4 bytes). Using this method
// makes possible to test the cases, where hashes collide.
// E.g. we can be sure that key "a1" and "a2" will collide.


TEST_CASE("HashTable") {

	Logger* logger = new_Logger();
	Logger_setLevel(logger,Logger_MUTE);
	HashTable* ht = new_HashTable();
	HashTable_setLogger(ht,logger);
	HashTable_setMethod(ht,Hasher_METHOD_DEBUG);


	SECTION("check empty state") {
		int elms = HashTable_getNumberOfElms(ht);
		REQUIRE(elms == 0);
	}


	SECTION("set then del") {

		HashTable_performSet(ht,(char*)"dakej",5,(char*)"davalue",7);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

		HashTable_performDel(ht,(char*)"dakej",5);
		REQUIRE(HashTable_getNumberOfElms(ht) == 0);

	}

	SECTION("set then set same key") {

		int result = HashTable_performSet(ht,(char*)"dakej",5,(char*)"davalue",7);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);
		REQUIRE(result == 2);

		result = HashTable_performSet(ht,(char*)"dakej",5,(char*)"voila",6);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);
		REQUIRE(result == 1);

		char* val;
		int len;
		HashTable_performGet(ht,&val,&len,(char*)"dakej",5);
		REQUIRE(0 == strcmp(val,"voila"));

	}

	SECTION("add items to the same slot") {

		int result = HashTable_performSet(ht,(char*)"a1",2,(char*)"a1-value",9);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);
		REQUIRE(result == 2);

		result = HashTable_performSet(ht,(char*)"a2",2,(char*)"a2-value",9);
		REQUIRE(HashTable_getNumberOfElms(ht) == 2);
		REQUIRE(result == 2);

		char* val;
		int len;

		HashTable_performGet(ht,&val,&len,(char*)"a1",2);
		REQUIRE(0 == strcmp(val,"a1-value"));
		REQUIRE(result == 2);

		HashTable_performGet(ht,&val,&len,(char*)"a2",2);
		REQUIRE(0 == strcmp(val,"a2-value"));
		REQUIRE(result == 2);

	}

	delete_HashTable(ht);

} // test case