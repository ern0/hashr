#ifndef SearchOptions_h_included
#define SearchOptions_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Utils.h"

// class SearchOptions

	#define SEARCHOPT_MAXRESULTS_DEFAULT 20

	#define SEARCHOPT_COUNT 111
	#define SEARCHOPT_SEARCH 99

	struct SearchOptions {

		// request
		char* patternData;
		int patternLength;
		int maxResults;
		char mode;
		int keyMatch;
		int valueMatch;
		
		// response
		int numberOfResults;

		// TODO

	};
	typedef struct SearchOptions SearchOptions;

	// public
	SearchOptions* new_SearchOptions();
	void delete_SearchOptions(SearchOptions* self);
	void SearchOptions_setPattern(SearchOptions* self,char* patternData,int patternLength);
	char* SearchOptions_getPatternData(SearchOptions* self);
	int SearchOptions_getPatternLength(SearchOptions* self);
	void SearchOptions_setCountMode(SearchOptions* self,int keyMatch,int valueMatch);
	void SearchOptions_setSearchMode(SearchOptions* self,int keyMatch,int valueMatch);
	int SearchOptions_isCountMode(SearchOptions* self);
	int SearchOptions_isSearchMode(SearchOptions* self);
	int SearchOptions_isKeyMatchMode(SearchOptions* self);
	int SearchOptions_isValueMatchMode(SearchOptions* self);
	void SearchOptions_setMaxResults(SearchOptions* self,int max);
	int SearchOptions_getNumberOfResults(SearchOptions* self);
	void SearchOptions_resetNumberOfResults(SearchOptions* self);

	// protected
	void SearchOptions_ctor(SearchOptions* self);
	void SearchOptions_dtor(SearchOptions* self);


#endif