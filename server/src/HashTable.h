#ifndef HashTable_h_included
#define HashTable_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RET

#include "Utils.h"
#include "Logger.h"
#include "HashItem.h"
#include "Hasher.h"

#define HashTable_CAPACITY_DEFAULT 8
#define HashTable_CAPACITY_MAX 0x10000

#define HashTable_SET_INSERTED 1
#define HashTable_SET_UPDATED 2
#define HashTable_GET_PROVIDED 3
#define HashTable_GET_NOT_FOUND 4
#define HashTable_DEL_DELETED 5
#define HashTable_DEL_ALREADY 6
#define HashTable_ZAP_ZAPPED 7
#define HashTable_ZAP_EMPTY 8


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
	HashItem** HashTable_findItem(HashTable* self,int hash,char* data,int lenght);
	int HashTable_getHash(HashTable* self,char* data,int len);

	// public
	HashTable* new_HashTable();
	void delete_HashTable(HashTable* self);
	void HashTable_setLogger(HashTable* self,Logger* logger);
	void HashTable_setMethod(HashTable* self,int method);
	int HashTable_getMethod(HashTable* self);
	int HashTable_setAndAdjustMethod(HashTable* self,int method);
	int HashTable_getCapacity(HashTable* self);
	int HashTable_getNumberOfElms(HashTable* self);
	void HashTable_outOfMemory(HashTable* self,int code);	
	void HashTable_dump(HashTable* self);	

	int HashTable_performSet(HashTable* self,char* keydata,int keylen,char* valdata,int vallen);
	int HashTable_performGet(HashTable* self,RET char** valdata,RET int* vallen,char* keydata,int keylen);
	int HashTable_performDel(HashTable* self,char* keydata,int keylen);
	int HashTable_performZap(HashTable* self);
	int HashTable_search(HashTable* self,RET HashItem** resultPtr,int maxResult,int keySearch,int valueSearch);


#endif