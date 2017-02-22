#include "SearchOptions.h"


	SearchOptions* new_SearchOptions() {

		SearchOptions* self;
		self = (SearchOptions*)malloc(sizeof(SearchOptions));
		if (self != NULL) SearchOptions_ctor(self);

		return self;
	} // new 


	void delete_SearchOptions(SearchOptions* self) {
		SearchOptions_dtor(self);
		free(self);
	} // delete 


	void SearchOptions_ctor(SearchOptions* self) {

		self->patternData = NULL;
		self->patternLength = -1;
		self->maxResults = -1;
		self->mode = -1;
		self->keyMatch = 0;
		self->valueMatch = 0;
		self->numberOfResults = 0;

	} // ctor


	void SearchOptions_dtor(SearchOptions* self) {
		// NOP
	} // dtor


	void SearchOptions_setPattern(SearchOptions* self,char* patternData,int patternLength) {
		self->patternData = patternData;
		self->patternLength = patternLength;
	} // setPattern()


	char* SearchOptions_getPatternData(SearchOptions* self) {
		return self->patternData;
	} // getPatternData()


	int SearchOptions_getPatternLength(SearchOptions* self) {
		return self->patternLength;
	} // getPatternLength()


	void SearchOptions_setCountMode(SearchOptions* self,int keyMatch,int valueMatch) {

		self->mode = SEARCHOPT_COUNT;

		self->keyMatch = keyMatch;
		self->valueMatch = valueMatch;

	} // setCountMode()


	void SearchOptions_setSearchMode(SearchOptions* self,int keyMatch,int valueMatch) {

		self->mode = SEARCHOPT_SEARCH;

		self->keyMatch = keyMatch;
		self->valueMatch = valueMatch;

	} // setSearchMode()


	int SearchOptions_isKeyMatchMode(SearchOptions* self) {
		return self->keyMatch;
	} // isKeyMatchMode()


	int SearchOptions_isValueMatchMode(SearchOptions* self) {
		return self->valueMatch;
	} // isValueMatchhMode()


	int SearchOptions_isCountMode(SearchOptions* self) {
		return ( self->mode == SEARCHOPT_COUNT );
	} // isCountMode()


	int SearchOptions_isSearchMode(SearchOptions* self) {
		return ( self->mode == SEARCHOPT_SEARCH );
	} // isSearchMode()


	void SearchOptions_setMaxResults(SearchOptions* self,int max) {
		if (max == -1) max = SEARCHOPT_MAXRESULTS_DEFAULT;
		self->maxResults = max;
	} // setSearchMax()


	void SearchOptions_resetNumberOfResults(SearchOptions* self) {
		self->numberOfResults = 0;
	} // resetNumberOfResults()	


	int SearchOptions_getNumberOfResults(SearchOptions* self) {
		return self->numberOfResults;
	} // getNumberOfResults()	
