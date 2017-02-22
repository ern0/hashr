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

		self->patternData = NULL
		self->patternLenght = -1;
		self->maxResult = -1;
		self->mode = -1;
		self->keySearch = 0;
		self->valueSearch = 0;
		self->numberOfResults = 0;

	} // ctor


	void SearchOptions_dtor(SearchOptions* self) {
		// NOP
	} // dtor



	void SearchOptions_setCountMode(SearchOptions* self,char keySearch,char valueSearch) {
		self->mode = SEARCHOPT_COUNT;
	} // setCountMode()


	void SearchOptions_setSearchMode(SearchOptions* self,char keySearch,char valueSearch) {
		self->mode = SEARCHOPT_SEARCH;
	} // setSearchMode()


	char SearchOptions_isCountMode(SearchOptions* self) {
		return ( self->mode == SEARCHOPT_COUNT );
	} // isCountMode()


	char SearchOptions_isSearchMode(SearchOptions* self) {
		return ( self->mode == SEARCHOPT_SEARCH );
	} // isSearchMode()


	int SearchOptions_getNumberOfResults(SearchOptions* self) {
		return self->numberOfResults;
	} // getNumberOfResults()	
