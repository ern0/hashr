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
		self->mode = -1;
		self->match = 0;
		self->limitStart = 0;
		self->limitItems = -1;
		self->numberOfResults = 0;
		self->numberOfMatching = 0;
		self->results = NULL;

	} // ctor


	void Search_dtor(Search* self) {
		Search_dropResults(self);
	} // dtor


	void Search_dropResults(Search* self) {
		
		if (self->results == NULL) return;

		free(self->results);
		self->results = NULL;

	} // dropResults()


	void Search_setPattern(Search* self,char* patternData,int patternLength) {
		self->patternData = patternData;
		self->patternLength = patternLength;
	} // setPattern()


	void Search_setCountMode(Search* self,int match) {

		self->mode = SEARCH_MODE_COUNT;
		self->match = match;

	} // setCountMode()


	void Search_setSearchMode(Search* self,int match) {

		self->mode = SEARCH_MODE_SEARCH;
		self->match = match;

	} // setSearchMode()


	int Search_isCountMode(Search* self) {
		return ( self->mode == SEARCH_MODE_COUNT );
	} // isCountMode()


	int Search_isSearchMode(Search* self) {
		return ( self->mode == SEARCH_MODE_SEARCH );
	} // isSearchMode()


	void Search_setLimitStart(Search* self,int limitStart) {
		self->limitStart = limitStart;
	} // setLimitStart()


	void Search_setLimitItems(Search* self,int limitItems) {
		self->limitItems = limitItems;
	} // setLimitItems()


	int Search_getNumberOfResults(Search* self) {
		return self->numberOfResults;
	} // getNumberOfResults()	


	HashItem* Search_getResult(Search* self,int index) {
		if (self->results == NULL) return NULL;
		return self->results[index];
	} // getResult()


	void Search_resetResults(Search* self,int numberOfElms) {

		self->numberOfElms = numberOfElms;

		self->numberOfChecked = 0;
		self->numberOfMatching = 0;
		self->numberOfResults = 0;

		Search_dropResults(self);

	} // resetNumberOfResults()	


	int Search_process(Search* self,HashItem* item) {

		// checkpoint: process item

		int found = 0;
		if ((self->match & SEARCH_MATCH_KEY) != 0) {
			found = HashItem_searchKey(item,self->patternData,self->patternLength);
		}
		if (!found) {
			if ((self->match & SEARCH_MATCH_VALUE) != 0) {
				found = HashItem_searchValue(item,self->patternData,self->patternLength);				
			}
		} // if not found

		self->numberOfChecked++;
		if (!found) return 0;

		// checkpoint: found

		if (self->mode == SEARCH_MODE_COUNT) {
			self->numberOfResults++;
			return 0;
		}

		// checkpoint: search mode

		if (self->limitItems > 0) {  // overprotection
			if (self->numberOfResults >= self->limitItems) return 1;
		}

		self->numberOfMatching++;
		if (self->numberOfMatching <= self->limitStart) return 0;

		// checkpoint: store

		if (self->numberOfResults == 0) {

			int remaining = self->numberOfElms - self->numberOfChecked;
			if ((self->limitItems > 0) && (self->limitItems < remaining)) {
				remaining = self->limitItems;
			}
			self->results = (HashItem**)malloc(sizeof(HashItem*) * remaining);
		} // if first item to store (lazy allocation)

		// checkpoint: memory allocated for results

		self->results[self->numberOfResults] = item;
		self->numberOfResults++;

		// checkpoint: item stored

		if (self->limitItems > 0) {
			if (self->numberOfResults >= self->limitItems) return 1;
		}

		// checkpoint: ready to process next item

		return 0;
	} // process()
