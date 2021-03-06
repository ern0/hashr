#ifndef Search_h_included
#define Search_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Utils.h"
#include "HashItem.h"

// class Search

	#define SEARCH_MAXRESULTS_DEFAULT 20

	#define SEARCH_MODE_COUNT 111
	#define SEARCH_MODE_SEARCH 99
	#define SEARCH_MATCH_KEY 0x01
	#define SEARCH_MATCH_VALUE 0x02

	struct Search {

		// request
		char* patternData;
		int patternLength;
		int mode;
		int match;
		int limitStart;
		int limitItems;

		// aux
		int numberOfElms;

		// response
		int numberOfChecked;
		int numberOfMatching;
		int numberOfResults;
		HashItem** results;

	};
	typedef struct Search Search;

	// public
	Search* new_Search();
	void delete_Search(Search* self);
	void Search_setPattern(Search* self,char* patternData,int patternLength);
	void Search_setCountMode(Search* self,int match);
	void Search_setSearchMode(Search* self,int match);
	int Search_isCountMode(Search* self);
	int Search_isSearchMode(Search* self);
	void Search_setLimitStart(Search* self,int limitStart);
	void Search_setLimitItems(Search* self,int limitItems);
	void Search_resetResults(Search* self,int numberOfElms);
	int Search_process(Search* self,HashItem* item);
	int Search_getNumberOfResults(Search* self);
	HashItem* Search_getResult(Search* self,int index);

	// protected
	void Search_ctor(Search* self);
	void Search_dtor(Search* self);
	void Search_dropResults(Search* self);


#endif