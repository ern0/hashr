#ifndef HashItem_h_included
#define HashItem_h_included

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Hasher.h"


// class HashItem

	// private
	struct HashItem;
	typedef struct HashItem HashItem;
	struct HashItem {
		HashItem* next;
		char* keyData;
		int keyLength;
		char* valueData;
		int valueLength;
		char method;
	};
	typedef struct HashItem HashItem;

	// private
	void HashItem_ctor(HashItem* self);
	void HashItem_dtor(HashItem* self);

	// public
	HashItem* new_HashItem();
	void delete_HashItem(HashItem* self);
	void HashItem_dump(HashItem* self);
	void HashItem_setMethod(HashItem* self,int method);
	int HashItem_setKey(HashItem* self,char* data,int length);
	char* HashItem_getKeyData(HashItem* self);
	int HashItem_getKeyLength(HashItem* self);
	int HashItem_setValue(HashItem* self,char* buffer,int length);
	char* HashItem_getValueData(HashItem* self);
	int HashItem_getValueLength(HashItem* self);
	HashItem** HashItem_getNextPtr(HashItem* self);
	HashItem* HashItem_getNext(HashItem* self);
	void HashItem_setNext(HashItem* self,HashItem* next);
	int HashItem_replaceValue(HashItem* self,char* data,int length);

#endif