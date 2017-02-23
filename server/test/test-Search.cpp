#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" {
	#include "../src/Utils.h"
	#include "../src/Logger.h"
	#include "../src/HashTable.h"
	#include "../src/Search.h"
}

// make array from constant literal
#define ARR(x) (char*)x,strlen((char*)x)+1


TEST_CASE("Search") {

	Logger* logger = new_Logger();
	Logger_setLevel(logger,Logger_MUTE);
	HashTable* ht = new_HashTable();
	HashTable_setLogger(ht,logger);
	HashTable_setMethod(ht,Hasher_METHOD_DEBUG);
	int result;
	HashItem* item;

	Search* search = new Search();


	SECTION("count and search on empty") {

		int elms = HashTable_getNumberOfElms(ht);
		REQUIRE(elms == 0);

		Search_setSearchMode(search,SEARCH_MATCH_KEY | SEARCH_MATCH_VALUE);
		Search_setPattern(search,(char*)"a",1);
		result = HashTable_search(ht,search);
		REQUIRE(result == HashTable_SEARCH_NOT_FOUND);
		result = Search_getNumberOfResults(search);
		REQUIRE(result == 0);

		Search_setCountMode(search,SEARCH_MATCH_KEY | SEARCH_MATCH_VALUE);
		Search_setPattern(search,(char*)"a",1);
		result = HashTable_search(ht,search);
		REQUIRE(result == HashTable_SEARCH_NOT_FOUND);
		result = Search_getNumberOfResults(search);
		REQUIRE(result == 0);

	}


	SECTION("insert some data") {

		HashTable_performSet(ht,ARR("cal"),ARR("the calendar"));
		HashTable_performSet(ht,ARR("01"),ARR("January"));
		HashTable_performSet(ht,ARR("02"),ARR("February"));
		HashTable_performSet(ht,ARR("03"),ARR("March"));
		HashTable_performSet(ht,ARR("04"),ARR("April"));
		HashTable_performSet(ht,ARR("05"),ARR("May"));
		HashTable_performSet(ht,ARR("06"),ARR("June"));
		HashTable_performSet(ht,ARR("07"),ARR("July"));
		HashTable_performSet(ht,ARR("08"),ARR("August"));
		HashTable_performSet(ht,ARR("09"),ARR("September"));
		HashTable_performSet(ht,ARR("10"),ARR("October"));
		HashTable_performSet(ht,ARR("11"),ARR("November"));
		HashTable_performSet(ht,ARR("12"),ARR("December"));

		REQUIRE(HashTable_getNumberOfElms(ht) == 13);

		SECTION("key search exact match") {

			Search_setPattern(search,ARR("02"));
			Search_setSearchMode(search,SEARCH_MATCH_KEY);
			result = HashTable_search(ht,search);
			REQUIRE(result == HashTable_SEARCH_PROVIDED);
			result = Search_getNumberOfResults(search);
			REQUIRE(result == 1);

			item = Search_getResult(search,0);
			REQUIRE(item != NULL);
			REQUIRE( HashItem_matchKey(item,ARR("02")) );

		}

		SECTION("key search not found") {

			Search_setPattern(search,ARR("a"));
			Search_setSearchMode(search,SEARCH_MATCH_KEY);
			result = HashTable_search(ht,search);
			REQUIRE(result == HashTable_SEARCH_NOT_FOUND);
			result = Search_getNumberOfResults(search);
			REQUIRE(result == 0);

		}

		SECTION("value search exact match") {

			Search_setPattern(search,ARR("November"));
			Search_setSearchMode(search,SEARCH_MATCH_VALUE);
			result = HashTable_search(ht,search);
			REQUIRE(result == HashTable_SEARCH_PROVIDED);
			result = Search_getNumberOfResults(search);
			REQUIRE(result == 1);

			item = Search_getResult(search,0);
			REQUIRE(item != NULL);
			REQUIRE( HashItem_matchValue(item,ARR("November")) );

		}

		SECTION("value search not found") {

			Search_setPattern(search,ARR("Julember"));
			Search_setSearchMode(search,SEARCH_MATCH_KEY);
			result = HashTable_search(ht,search);
			REQUIRE(result == HashTable_SEARCH_NOT_FOUND);
			result = Search_getNumberOfResults(search);
			REQUIRE(result == 0);

		}

		SECTION("key search partial match") {

			Search_setPattern(search,ARR("2"));
			Search_setSearchMode(search,SEARCH_MATCH_KEY);
			result = HashTable_search(ht,search);
			REQUIRE(result == HashTable_SEARCH_PROVIDED);
			result = Search_getNumberOfResults(search);
			REQUIRE(result == 2);

			item = Search_getResult(search,0);
			REQUIRE(item != NULL);
			REQUIRE( HashItem_matchKey(item,ARR("02")) );

			item = Search_getResult(search,1);
			REQUIRE(item != NULL);
			REQUIRE( HashItem_matchKey(item,ARR("12")) );

		}

		SECTION("key search with existing values fail") {

			Search_setPattern(search,ARR("ember"));
			Search_setSearchMode(search,SEARCH_MATCH_KEY);
			result = HashTable_search(ht,search);
			REQUIRE(result == HashTable_SEARCH_NOT_FOUND);

		}

		SECTION("value search partial match") {

			Search_setPattern(search,ARR("ember"));
			Search_setSearchMode(search,SEARCH_MATCH_VALUE);
			result = HashTable_search(ht,search);
			REQUIRE(result == HashTable_SEARCH_PROVIDED);
			result = Search_getNumberOfResults(search);
			REQUIRE(result == 3);

			item = Search_getResult(search,0);
			REQUIRE(item != NULL);
			REQUIRE( HashItem_matchValue(item,ARR("September")) );

			item = Search_getResult(search,1);
			REQUIRE(item != NULL);
			REQUIRE( HashItem_matchValue(item,ARR("November")) );

			item = Search_getResult(search,2);
			REQUIRE(item != NULL);
			REQUIRE( HashItem_matchValue(item,ARR("December")) );

		}

		SECTION("key and value search match") {

			Search_setPattern(search,ARR("cal"));
			Search_setSearchMode(search,SEARCH_MATCH_KEY | SEARCH_MATCH_VALUE);
			result = HashTable_search(ht,search);
			REQUIRE(result == HashTable_SEARCH_PROVIDED);
			result = Search_getNumberOfResults(search);
			REQUIRE(result == 1);

		}

	}

	delete_Search(search);
	delete_HashTable(ht);

} // test case
