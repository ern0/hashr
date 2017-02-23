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

	struct Search {

		// request
		char* patternData;
		int patternLength;
		char mode;
		int keyMatch;
		int valueMatch;
		int limitStart;
		int limitItems;
		
		// response
		int numberOfResults;
		HashItem** results;

	};
	typedef struct Search Search;

	// public
	Search* new_Search();
	void delete_Search(Search* self);
	void Search_setPattern(Search* self,char* patternData,int patternLength);
	char* Search_getPatternData(Search* self);
	int Search_getPatternLength(Search* self);
	void Search_setCountMode(Search* self,int keyMatch,int valueMatch);
	void Search_setSearchMode(Search* self,int keyMatch,int valueMatch);
	int Search_isCountMode(Search* self);
	int Search_isSearchMode(Search* self);
	int Search_isKeyMatchMode(Search* self);
	int Search_isValueMatchMode(Search* self);
	int Search_getNumberOfResults(Search* self);
	void Search_setLimitStart(Search* self,int limitStart);
	int Search_getLimitStart(Search* self);
	void Search_setLimitItems(Search* self,int limitItems);
	int Search_getLimitItems(Search* self);
	void Search_resetResults(Search* self);
	void Search_incNumberOfResults(Search* self);
	void Search_addResult(Search* self,HashItem* item,int remaining);
	HashItem* Search_getResult(Search* self,int index);

	// protected
	void Search_ctor(Search* self);
	void Search_dtor(Search* self);


#endif