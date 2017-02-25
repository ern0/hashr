#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "../src/Utils.h"
	#include "../src/Logger.h"
	#include "../src/HashTable.h"
	#include "../src/Hasher.h"
}

// make array from constant literal
#define ARR(x) (char*)x,strlen((char*)x)+1

// CAUTION: These tests are using the "debug0" and "debug1" hash 
// methods. They takes the first or second character, respectively,
// and return it as the hash code, so data can be sent to a 
// specific slot. Using this method makes possible to test the 
// cases where hashes collide, test the linked list's behaviors.
// E.g. we can be sure that key "a1" and "a2" will collide with
// "debug0", but they will be separate slots with "debug1".


TEST_CASE("HashTable reorg") {

	Logger* logger = new_Logger();
	Logger_setLevel(logger,Logger_MUTE);
	HashTable* ht = new_HashTable();
	HashTable_setLogger(ht,logger);
	int result;


	SECTION("empty, method change") {

		REQUIRE( HashTable_getNumberOfElms(ht) == 0 );
		REQUIRE( HashTable_getMethod(ht) == Hasher_METHOD_DEFAULT );
	
		result = HashTable_performReorg(ht,Hasher_METHOD_DEBUG0);
		REQUIRE( result == HashTable_REORG_PERFORMED );
		REQUIRE( HashTable_getMethod(ht) == Hasher_METHOD_DEBUG0 );

		result = HashTable_performReorg(ht,Hasher_METHOD_DEBUG0);
		REQUIRE( result == HashTable_REORG_UNCHANGED );
		REQUIRE( HashTable_getMethod(ht) == Hasher_METHOD_DEBUG0 );

	}

	SECTION("empty, check starting capacity") {

		REQUIRE( HashTable_getNumberOfElms(ht) == 0 );
		HashTable_performSet(ht,ARR("key"),ARR("value"));
		REQUIRE( HashTable_getNumberOfElms(ht) == 1 );
		REQUIRE( HashTable_getCapacity(ht) == HashTable_CAPACITY_MIN );

	}

	SECTION("fill minimum capacity / 2") {

		REQUIRE( HashTable_getNumberOfElms(ht) == 0 );

		for (int i = 0; i < HashTable_CAPACITY_MIN / 2; i++) {

			char key[80];
			sprintf(key,"%d",i);
			char value[80];
			sprintf(value,"value-%04d",i);
			result = HashTable_performSet(ht,key,strlen(key),value,strlen(value));
			REQUIRE(result == HashTable_SET_INSERTED);

		} // for fill

		REQUIRE( HashTable_getNumberOfElms(ht) == HashTable_CAPACITY_MIN / 2);
		REQUIRE( HashTable_getCapacity(ht) == HashTable_CAPACITY_MIN);


		SECTION("exceeding min/2 cause expanding capacity") {

			result = HashTable_performSet(ht,ARR("K"),ARR("V"));
			REQUIRE(result == HashTable_SET_INSERTED);
			REQUIRE( HashTable_getCapacity(ht) == HashTable_CAPACITY_MIN * 2);

		}

		SECTION("not exceeding min/2, only updating, capacity unchanged") {

			int i = 0;
			char key[80];
			sprintf(key,"%d",i);
			result = HashTable_performSet(ht,key,strlen(key),ARR("value"));
			REQUIRE(result == HashTable_SET_UPDATED);
			REQUIRE( HashTable_getCapacity(ht) == HashTable_CAPACITY_MIN);

		}

		SECTION("reorg, capacity does not change") {

			result = HashTable_performReorg(ht,Hasher_METHOD_DEBUG1);
			REQUIRE( result == HashTable_REORG_PERFORMED );
		
		}

	}


	SECTION("fill minimum capacity") {

		REQUIRE( HashTable_getNumberOfElms(ht) == 0 );

		for (int i = 0; i < HashTable_CAPACITY_MIN; i++) {

			char key[80];
			sprintf(key,"%d",i);
			char value[80];
			sprintf(value,"value-%04d",i);
			result = HashTable_performSet(ht,key,strlen(key),value,strlen(value));
			REQUIRE(result == HashTable_SET_INSERTED);

		} // for fill

		REQUIRE( HashTable_getNumberOfElms(ht) == HashTable_CAPACITY_MIN );
		REQUIRE( HashTable_getCapacity(ht) > HashTable_CAPACITY_MIN);

		
		SECTION("zap, capacity changes to minimum") {

			result = HashTable_performZap(ht);
			REQUIRE( result == HashTable_ZAP_ZAPPED );

			REQUIRE( HashTable_getNumberOfElms(ht) == 0 );
			REQUIRE( HashTable_getCapacity(ht) == HashTable_CAPACITY_MIN );

		}

		SECTION("del all, capacity changes to minimum") {

			for (int i = 0; i < HashTable_CAPACITY_MIN; i++) {

				char key[80];
				sprintf(key,"%d",i);
				result = HashTable_performDel(ht,key,strlen(key));
				REQUIRE(result == HashTable_DEL_DELETED);

			} // for fill

			REQUIRE( HashTable_getNumberOfElms(ht) == 0 );
			REQUIRE( HashTable_getCapacity(ht) == HashTable_CAPACITY_MIN );

		}

		SECTION("del half of items, capacity unchanged") {

			REQUIRE( HashTable_getCapacity(ht) == HashTable_CAPACITY_MIN * 2 );

			for (int i = 0; i < HashTable_CAPACITY_MIN / 2; i++) {

				char key[80];
				sprintf(key,"%d",i);
				result = HashTable_performDel(ht,key,strlen(key));
				REQUIRE(result == HashTable_DEL_DELETED);

			} // for fill

			REQUIRE( HashTable_getNumberOfElms(ht) == HashTable_CAPACITY_MIN / 2 );
			REQUIRE( HashTable_getCapacity(ht) == HashTable_CAPACITY_MIN * 2 );

		}

	}

	delete_HashTable(ht);

} // test cas