#include "HashTable.h"


	HashTable* new_HashTable() {

		HashTable* self;
		self = (HashTable*)malloc(sizeof(HashTable));
		if (self != NULL) HashTable_ctor(self);

		return self;
	} // new 


	void delete_HashTable(HashTable* self) {
		HashTable_dtor(self);
		free(self);
	} // delete 


	void HashTable_ctor(HashTable* self) {

		self->method = Hasher_METHOD_DEFAULT;
		self->capacity = Utils_roundUp2Power(HashTable_CAPACITY_DEFAULT);
		self->hashMask = Utils_calcHashMask(self->capacity);		
		self->numberOfElms = 0;

		self->items = (HashItem**)malloc(self->capacity * sizeof(HashItem*));
		if (self->items == NULL) HashTable_outOfMemory(self,2801);

		for (int i = 0; i < self->capacity; i++) {
			self->items[i] = NULL;
		}

	} // ctor


	void HashTable_dtor(HashTable* self) {
		// NOP
	} // dtor


	void HashTable_outOfMemory(HashTable* self,int code) {		

		Logger_log(
			self->logger
			,Logger_FATAL
			,code,"Out of memory"
		);
		exit(2);

	} // outOfMemory()


	void HashTable_setLogger(HashTable* self,Logger* logger) {
		self->logger = logger;
	} // setLogger()


	int HashTable_getMethod(HashTable* self) {
		return self->method;
	} // getMethod()


	int HashItem_setAndAdjustMethod(HashTable* self,int method) {

		if (method < 1) return self->method;
		if (method > Hasher_METHOD_MAX) return self->method;

		self->method = method;

		return method;
	} // setAndAdjustMethod()


	int HashTable_getCapacity(HashTable* self) {
		return self->capacity;
	} // getCapacity()


	int HashTable_getNumberOfElms(HashTable* self) {
		return self->numberOfElms;
	} // getNumberOfElms()


	void HashTable_dump(HashTable* self) {

		for (int i = 0; i < self->capacity; i++) {
			HashItem* item = self->items[i];
			if ( (self->capacity > 32) && (item == NULL)) continue;

			printf("%04X: ",i);
			if (item != NULL) HashItem_dump(item);
			printf("\n");
		} // foreach items

	} // dump()


	HashItem** HashTable_findItem(HashTable* self,int hash,char* data,int length) {

		HashItem** itemPtr = &self->items[hash];
		while (1) {
			
			if (*itemPtr == NULL) return NULL;
			
			if ( HashItem_getKeyLength(*itemPtr) == length ) {
				if (0 == memcmp(HashItem_getKeyData(*itemPtr),data,length)) return itemPtr;
			} // if length match

			itemPtr = HashItem_getNextPtr(*itemPtr);

		} // loop

		return NULL;
	} // findItem()

	
	int HashTable_performSet(HashTable* self,char* keydata,int keylen,char* valdata,int vallen) {

		int hash = Hasher_hash(self->method,keydata,keylen);
		hash &= self->hashMask;

		// if there's an item with the same key, replace value only
		HashItem** oldItemPtr = HashTable_findItem(self,hash,keydata,keylen);
		if (oldItemPtr != NULL) {
			HashItem_replaceValue(*oldItemPtr,valdata,vallen);
			return 2;
		}

		HashItem* item = new_HashItem();
		if (item == NULL) HashTable_outOfMemory(self,2802);
		HashItem_setMethod(item,self->method);
		if (-1 == HashItem_setKey(item,keydata,keylen)) HashTable_outOfMemory(self,2802);
		if (-1 == HashItem_setValue(item,valdata,vallen)) HashTable_outOfMemory(self,2802);

		// put new item into slot, to the end of the linked list
		HashItem** newItemPtr = &self->items[hash];
		while (*newItemPtr != NULL) {
			newItemPtr = HashItem_getNextPtr(*newItemPtr);
		}
		*newItemPtr = item;		

		self->numberOfElms++;

		return 1;
	} // performSet()

