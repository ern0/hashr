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

	SECTION("set and get, then del and get") {

		int result = HashTable_performSet(ht,(char*)"dakej",5,(char*)"davalue",7);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

		result = HashTable_performDel(ht,(char*)"dakej",5);
		REQUIRE(result == HashTable_DEL_DELETED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 0);

		char* val;
		int len;
		result = HashTable_performGet(ht,&val,&len,(char*)"dakej",5);
		REQUIRE(result == HashTable_GET_NOT_FOUND);

	}

	SECTION("del non-existent") {

		int result = HashTable_performSet(ht,(char*)"yyyy",5,(char*)"davalue",7);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

		result = HashTable_performDel(ht,(char*)"zzzz",5);
		REQUIRE(result == HashTable_DEL_ALREADY);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

		result = HashTable_performDel(ht,(char*)"wwww",5);
		REQUIRE(result == HashTable_DEL_ALREADY);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

	}

	SECTION("set and get, then del, then del again") {

		int result = HashTable_performSet(ht,(char*)"dakej",5,(char*)"davalue",7);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

		result = HashTable_performDel(ht,(char*)"dakej",5);
		REQUIRE(result == HashTable_DEL_DELETED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 0);

		result = HashTable_performDel(ht,(char*)"dakej",5);
		REQUIRE(result == HashTable_DEL_ALREADY);
		REQUIRE(HashTable_getNumberOfElms(ht) == 0);

	}

	SECTION("set, then update with shorter") {

		int result = HashTable_performSet(ht,(char*)"dakej",5,(char*)"davalue",7);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

		result = HashTable_performSet(ht,(char*)"dakej",5,(char*)"yo",3);
		REQUIRE(result == HashTable_SET_UPDATED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

		char* val;
		int len;
		result = HashTable_performGet(ht,&val,&len,(char*)"dakej",5);
		REQUIRE(result == HashTable_GET_PROVIDED);
		REQUIRE(0 == strcmp(val,"yo"));

	}

	SECTION("add items to the same slot") {

		int result = HashTable_performSet(ht,(char*)"a1",2,(char*)"a1-value",9);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

		result = HashTable_performSet(ht,(char*)"a2",2,(char*)"a2-value",9);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 2);

		char* val;
		int len;
		result = HashTable_performGet(ht,&val,&len,(char*)"a1",2);
		REQUIRE(result == HashTable_GET_PROVIDED);
		REQUIRE(0 == strcmp(val,"a1-value"));

		result = HashTable_performGet(ht,&val,&len,(char*)"a2",2);
		REQUIRE(result == HashTable_GET_PROVIDED);
		REQUIRE(0 == strcmp(val,"a2-value"));

	}

	SECTION("chain tests") {

		int result = HashTable_performSet(ht,(char*)"xh",2,(char*)"x1-value",9);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

		result = HashTable_performSet(ht,(char*)"x2",2,(char*)"x2-value",9);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 2);

		result = HashTable_performSet(ht,(char*)"x3",2,(char*)"x3-value",9);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 3);

		result = HashTable_performSet(ht,(char*)"xt",2,(char*)"x4-value",9);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 4);

		SECTION("del chain head") {

			int result = HashTable_performDel(ht,(char*)"xh",2);
			REQUIRE(result == HashTable_DEL_DELETED);
			REQUIRE(HashTable_getNumberOfElms(ht) == 3);

			result = HashTable_performDel(ht,(char*)"xh",2);
			REQUIRE(result == HashTable_DEL_ALREADY);
			REQUIRE(HashTable_getNumberOfElms(ht) == 3);

		}

		SECTION("del chain tail") {

			int result = HashTable_performDel(ht,(char*)"xt",2);
			REQUIRE(result == HashTable_DEL_DELETED);
			REQUIRE(HashTable_getNumberOfElms(ht) == 3);

			result = HashTable_performDel(ht,(char*)"xt",2);
			REQUIRE(result == HashTable_DEL_ALREADY);
			REQUIRE(HashTable_getNumberOfElms(ht) == 3);

		}

		SECTION("del chain 2nd") {

			int result = HashTable_performDel(ht,(char*)"x2",2);
			REQUIRE(result == HashTable_DEL_DELETED);
			REQUIRE(HashTable_getNumberOfElms(ht) == 3);

			result = HashTable_performDel(ht,(char*)"x2",2);
			REQUIRE(result == HashTable_DEL_ALREADY);
			REQUIRE(HashTable_getNumberOfElms(ht) == 3);

		}

		char* val;
		int len;
		result = HashTable_performGet(ht,&val,&len,(char*)"x3",2);
		REQUIRE(result == HashTable_GET_PROVIDED);
		REQUIRE(0 == strcmp(val,"x3-value"));

	}

	SECTION("zap on empty table") {

		int result = HashTable_performZap(ht);
		REQUIRE(result == HashTable_ZAP_EMPTY);
		REQUIRE(HashTable_getNumberOfElms(ht) == 0);

	}

	SECTION("zap a bunch of items") {

		int result = HashTable_performSet(ht,(char*)"x1",2,(char*)"x1-value",9);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 1);

		result = HashTable_performSet(ht,(char*)"x2",2,(char*)"x2-value",9);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 2);

		result = HashTable_performSet(ht,(char*)"x3",2,(char*)"x3-value",9);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 3);

		result = HashTable_performSet(ht,(char*)"yy",2,(char*)"yy-value",9);
		REQUIRE(result == HashTable_SET_INSERTED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 4);

		result = HashTable_performSet(ht,(char*)"yy",2,(char*)"updated",8);
		REQUIRE(result == HashTable_SET_UPDATED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 4);

		result = HashTable_performZap(ht);
		REQUIRE(result == HashTable_ZAP_ZAPPED);
		REQUIRE(HashTable_getNumberOfElms(ht) == 0);

		result = HashTable_performDel(ht,(char*)"x2",2);
		REQUIRE(result == HashTable_DEL_ALREADY);
		REQUIRE(HashTable_getNumberOfElms(ht) == 0);

		result = HashTable_performZap(ht);
		REQUIRE(result == HashTable_ZAP_EMPTY);
		REQUIRE(HashTable_getNumberOfElms(ht) == 0);

	}

	delete_HashTable(ht);

} // test case
