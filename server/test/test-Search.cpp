#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "../src/Utils.h"
	#include "../src/Logger.h"
	#include "../src/HashTable.h"
	#include "../src/Search.h"
}


TEST_CASE("Search") {

	Logger* logger = new_Logger();
	Logger_setLevel(logger,Logger_MUTE);
	HashTable* ht = new_HashTable();
	HashTable_setLogger(ht,logger);
	HashTable_setMethod(ht,Hasher_METHOD_DEBUG);
	int result;

	Search* search = new Search();


	SECTION("count and search on empty") {

		int elms = HashTable_getNumberOfElms(ht);
		REQUIRE(elms == 0);

		Search_setSearchMode(search,1,1);
		Search_setPattern(search,(char*)"a",1);
		result = HashTable_search(ht,search);
		REQUIRE(result == HashTable_SEARCH_NOT_FOUND);
		result = Search_getNumberOfResults(search);
		REQUIRE(result == 0);

		Search_setCountMode(search,1,1);
		Search_setPattern(search,(char*)"a",1);
		result = HashTable_search(ht,search);
		REQUIRE(result == HashTable_SEARCH_NOT_FOUND);
		result = Search_getNumberOfResults(search);
		REQUIRE(result == 0);

	}


	SECTION("insert some data") {

		HashTable_performSet(ht,(char*)"01",3,(char*)"January",8);
		HashTable_performSet(ht,(char*)"02",3,(char*)"February",9);
		HashTable_performSet(ht,(char*)"03",3,(char*)"March",6);
		HashTable_performSet(ht,(char*)"04",3,(char*)"April",6);
		HashTable_performSet(ht,(char*)"05",3,(char*)"May",4);
		HashTable_performSet(ht,(char*)"06",3,(char*)"June",5);
		HashTable_performSet(ht,(char*)"07",3,(char*)"July",5);
		HashTable_performSet(ht,(char*)"08",3,(char*)"August",7);
		HashTable_performSet(ht,(char*)"09",3,(char*)"September",10);
		HashTable_performSet(ht,(char*)"10",3,(char*)"October",8);
		HashTable_performSet(ht,(char*)"11",3,(char*)"November",9);
		HashTable_performSet(ht,(char*)"12",3,(char*)"December",7);

		REQUIRE(HashTable_getNumberOfElms(ht) == 12);

		SECTION("key search") {

			//HashTable_performSet(ht,(char*)"12",3,(char*)"December",7);

		}

	}

	delete_Search(search);
	delete_HashTable(ht);

} // test case
