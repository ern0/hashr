#include "Search.h"


	Search* new_Search() {

		Search* self;
		self = (Search*)malloc(sizeof(Search));
		if (self != NULL) Search_ctor(self);

		return self;
	} // new 


	void delete_Search(Search* self) {
		Search_dtor(self);
		free(self);
	} // delete 


	void Search_ctor(Search* self) {

		self->patternData = NULL;
		self->patternLength = -1;
		self->maxResults = -1;
		self->mode = -1;
		self->keyMatch = 0;
		self->valueMatch = 0;
		self->numberOfResults = 0;
		self->results = NULL;

	} // ctor


	void Search_dtor(Search* self) {
		if (self->results != NULL) free(self->results);
	} // dtor


	void Search_setPattern(Search* self,char* patternData,int patternLength) {
		self->patternData = patternData;
		self->patternLength = patternLength;
	} // setPattern()


	char* Search_getPatternData(Search* self) {
		return self->patternData;
	} // getPatternData()


	int Search_getPatternLength(Search* self) {
		return self->patternLength;
	} // getPatternLength()


	void Search_setCountMode(Search* self,int keyMatch,int valueMatch) {

		self->mode = SEARCH_MODE_COUNT;

		self->keyMatch = keyMatch;
		self->valueMatch = valueMatch;

	} // setCountMode()


	void Search_setSearchMode(Search* self,int keyMatch,int valueMatch) {

		self->mode = SEARCH_MODE_SEARCH;

		self->keyMatch = keyMatch;
		self->valueMatch = valueMatch;

	} // setSearchMode()


	int Search_isKeyMatchMode(Search* self) {
		return self->keyMatch;
	} // isKeyMatchMode()


	int Search_isValueMatchMode(Search* self) {
		return self->valueMatch;
	} // isValueMatchhMode()


	int Search_isCountMode(Search* self) {
		return ( self->mode == SEARCH_MODE_COUNT );
	} // isCountMode()


	int Search_isSearchMode(Search* self) {
		return ( self->mode == SEARCH_MODE_SEARCH );
	} // isSearchMode()


	void Search_setMaxResults(Search* self,int max) {
		if (max == -1) max = SEARCH_MAXRESULTS_DEFAULT;
		self->maxResults = max;
	} // setSearchMax()


	void Search_resetResults(Search* self) {
		self->numberOfResults = 0;
		self->results = NULL;
	} // resetNumberOfResults()	


	int Search_getNumberOfResults(Search* self) {
		return self->numberOfResults;
	} // getNumberOfResults()	


	void Search_incNumberOfResults(Search* self) {
		self->numberOfResults++;
	} // incNumberOfResults()


	void Search_addResult(Search* self,HashItem* item,int remaining) {

		if (self->numberOfResults == 0) {
			self->results = (HashItem**)malloc(sizeof(HashItem*) * remaining);
		}

		self->results[self->numberOfResults++] = item;

	} // addResult()


	HashItem* Search_getResult(Search* self,int index) {
		if (self->results == NULL) return NULL;
		return self->results[index];
	} // getResult()