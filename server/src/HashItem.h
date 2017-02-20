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
	int HashItem_setValue(HashItem* self,char* buffer,int length);
	int HashItem_replaceValue(HashItem* self,char* data,int length);

#endif