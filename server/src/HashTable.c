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


	int HashTable_getHash(HashTable* self,

		char* data,int len) {

		int hash = Hasher_hash(self->method,data,len);
		hash &= self->hashMask;

		return hash;
	} // getHashIndex()


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

		int hash = HashTable_getHash(self,keydata,keylen);

		// if there's an item with the same key, replace value only
		HashItem** oldItemPtr = HashTable_findItem(self,hash,keydata,keylen);
		if (oldItemPtr != NULL) {
			HashItem_replaceValue(*oldItemPtr,valdata,vallen);
			return 1;
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

		return 2;
	} // performSet()


	int HashTable_performGet(HashTable* self,RET char** valdata,RET int* vallen,char* keydata,int keylen) {

		int hash = HashTable_getHash(self,keydata,keylen);

		HashItem** itemPtr = HashTable_findItem(self,hash,keydata,keylen);
		if (itemPtr == NULL) return 2;

		*valdata = HashItem_getValueData(*itemPtr);
		*vallen = HashItem_getValueLength(*itemPtr);

		return 1;
	} // performGet()


	int HashTable_performDel(HashTable* self,char* keydata,int keylen) {

		int hash = HashTable_getHash(self,keydata,keylen);

		HashItem** itemPtr = HashTable_findItem(self,hash,keydata,keylen);
		if (itemPtr == NULL) return 2;

		HashItem* item = *itemPtr;
		HashItem* saved = HashItem_getNext(item);
		HashItem* head = self->items[hash];

		if (item == head) {
			self->items[hash] = saved;
		} // if head 

		else {

			HashItem* prev = head;
			while (1) {
				HashItem* next = HashItem_getNext(prev);
				if (next == item) break;
				prev = next;
			} // scan linked list

			HashItem_setNext(prev,saved);

		} // else not head

		delete_HashItem(item);
		--self->numberOfElms;

		return 1;
	} // performDel()
