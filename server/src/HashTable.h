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
#include "Search.h"

#define HashTable_CAPACITY_MIN 4
#define HashTable_CAPACITY_MAX (1024*1024)

#define HashTable_CAPACITY_EXPAND 1
#define HashTable_CAPACITY_SHRINK 2

#define HashTable_SET_INSERTED 11
#define HashTable_SET_UPDATED 12
#define HashTable_GET_PROVIDED 23
#define HashTable_GET_NOT_FOUND 20
#define HashTable_DEL_DELETED 35
#define HashTable_DEL_ALREADY 36
#define HashTable_ZAP_ZAPPED 45
#define HashTable_ZAP_EMPTY 46
#define HashTable_SEARCH_PROVIDED 53
#define HashTable_SEARCH_NOT_FOUND 50
#define HashTable_REORG_PERFORMED 65
#define HashTable_REORG_UNCHANGED 66

// class HashTable

	// private
	struct HashTable {
		Logger* logger;
		int method;
		int capacity;
		int minCapacity;
		int hashMask;
		int numberOfElms;
		HashItem** items;
		int reorgMethod;
		int reorgCapacity;
		char reorgMark;
		char lastCommandEffect;
	};
	typedef struct HashTable HashTable;

	// protected
	void HashTable_ctor(HashTable* self);
	void HashTable_dtor(HashTable* self);
	void HashTable_outOfMemory(HashTable* self,int code);	
	HashItem** HashTable_findItem(HashTable* self,int hash,char* data,int lenght);
	int HashTable_getHash(HashTable* self,char* data,int len);
	void HashTable_resizeMemory(HashTable* self,int capacity);
	void HashTable_moveItems(HashTable* self);

	// public
	HashTable* new_HashTable();
	void delete_HashTable(HashTable* self);
	void HashTable_setLogger(HashTable* self,Logger* logger);
	void HashTable_setMethod(HashTable* self,int method);
	int HashTable_getMethod(HashTable* self);
	int HashTable_getMinCapacity(HashTable* self);
	int HashTable_getCapacity(HashTable* self);
	int HashTable_getNumberOfElms(HashTable* self);
	int HashTable_getCollisionPercent(HashTable* self);
	/* static */ int HashTable_applyCapacityLimits(int capacity);
	void HashTable_calcMinCapacity(HashTable* self,int capacity);
	int HashTable_calcOptimalCapacity(HashTable* self);
	void HashTable_dump(HashTable* self);	

	int HashTable_performSet(HashTable* self,char* keydata,int keylen,char* valdata,int vallen);
	int HashTable_performGet(HashTable* self,RET char** valdata,RET int* vallen,char* keydata,int keylen);
	int HashTable_performDel(HashTable* self,char* keydata,int keylen);
	int HashTable_performZap(HashTable* self);
	int HashTable_performSearch(HashTable* self,Search* search);
	int HashTable_performReorg(HashTable* self,int method);


#endif