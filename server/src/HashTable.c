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


	void HashTable_setMethod(HashTable* self,int method) {

		if ((self->numberOfElms != 0) && (self->method != method)) {

			Logger_log(
				self->logger
				,Logger_FATAL
				,2809,"Internal error: setting hash method is not allowed"
			);
			exit(2);

		} // if not empty

		self->method = method;

	} // setMethod()


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


	int HashTable_getHash(HashTable* self,char* data,int len) {

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
			return HashTable_SET_UPDATED;
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

		return HashTable_SET_INSERTED;
	} // performSet()


	int HashTable_performGet(HashTable* self,RET char** valdata,RET int* vallen,char* keydata,int keylen) {

		int hash = HashTable_getHash(self,keydata,keylen);

		HashItem** itemPtr = HashTable_findItem(self,hash,keydata,keylen);
		if (itemPtr == NULL) return HashTable_GET_NOT_FOUND;

		*valdata = HashItem_getValueData(*itemPtr);
		*vallen = HashItem_getValueLength(*itemPtr);

		return HashTable_GET_PROVIDED;
	} // performGet()


	int HashTable_performDel(HashTable* self,char* keydata,int keylen) {

		int hash = HashTable_getHash(self,keydata,keylen);

		HashItem** itemPtr = HashTable_findItem(self,hash,keydata,keylen);
		if (itemPtr == NULL) return HashTable_DEL_ALREADY;

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

		return HashTable_DEL_DELETED;
	} // performDel()


	int HashTable_performZap(HashTable* self) {

		if (self->numberOfElms == 0) return HashTable_ZAP_EMPTY;

		for (int i = 0; i < self->capacity; i++) {

			HashItem* item = self->items[i];
			if (item == NULL) continue;

			// delete head

			HashItem* next = HashItem_getNext(item);
			delete_HashItem(item);
			self->items[i] = NULL;

			--self->numberOfElms;
			if (self->numberOfElms == 0) break;

			// delete linked list

			while (1) {
				if (next == NULL) break;
				HashItem* current = next;

				next = HashItem_getNext(current);
				delete_HashItem(current);

				--self->numberOfElms;
			} // while scan linked list

			if (self->numberOfElms == 0) break;

		} // foreach items

		return HashTable_ZAP_ZAPPED;
	} // performZap()


	int HashTable_search(HashTable* self,SearchOptions* opts) {

		opts->numberOfResults = 0;
		if (self->numberOfElms == 0) return HashTable_SEARCH_NOT_FOUND;

		for (int i = 0; i < self->capacity; i++) {

			HashItem* item = self->items[i];
			if (item == NULL) continue;

			
			//int result = HashTable_searchItem(self,item,opts);

		} // foreach slot

		return HashTable_SEARCH_NOT_FOUND;
	} // search()


	//int HashTable_searchItem(self,)
