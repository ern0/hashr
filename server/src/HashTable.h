#ifndef HashTable_h_included
#define HashTable_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Utils.h"
#include "Logger.h"
#include "HashItem.h"
#include "Hasher.h"

#define HashTable_CAPACITY_DEFAULT 8
#define HashTable_CAPACITY_MAX 0x10000
#define HashTable_METHOD_DEFAULT 1

#define HashTable_METHOD_DEBUG 1


// class HashTable

	// private
	struct HashTable {
		Logger* logger;
		int method;
		int capacity;
		int hashMask;
		int numberOfElms;
		HashItem** items;
	};
	typedef struct HashTable HashTable;

	// protected
	void HashTable_ctor(HashTable* self);
	void HashTable_dtor(HashTable* self);

	// public
	HashTable* new_HashTable();
	void delete_HashTable(HashTable* self);
	void HashTable_setLogger(HashTable* self,Logger* logger);
	int HashTable_getMethod(HashTable* self);
	int HashTable_getCapacity(HashTable* self);
	int HashTable_getNumberOfElms(HashTable* self);
	int HashTable_performSet(HashTable* self,char* keybuf,int keylen,char* valbuf,int vallen);
	void HashTable_dump(HashTable* self);	

#endif