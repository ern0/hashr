#ifndef HashTable_h_included
#define HashTable_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Utils.h"
#include "Logger.h"


// class HashTable

	// private
	struct HashTable {
		Logger* logger;
	};
	typedef struct HashTable HashTable;

	// public
	HashTable* new_HashTable();
	void delete_HashTable(HashTable* self);
	void HashTable_setLogger(HashTable* self,Logger* logger);

	// protected
	void HashTable_ctor(HashTable* self);
	void HashTable_dtor(HashTable* self);

#endif