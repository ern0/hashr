#include "HashTable.h"


	inline void HashTable_linkItem(HashTable* self,HashItem* item,int hash);


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

		self->numberOfElms = 0;
		self->reorgMark = 0;
		self->lastCommandEffect = HashTable_CAPACITY_EXPAND;
		self->method = Hasher_METHOD_DEFAULT;
		self->capacity = HashTable_applyCapacityLimits(HashTable_CAPACITY_MIN);
		self->minCapacity = HashTable_applyCapacityLimits(HashTable_CAPACITY_MIN);
		self->hashMask = Utils_calcHashMask(self->capacity);		

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


	int HashTable_getMinCapacity(HashTable* self) {
		return self->minCapacity;
	} // getMinCapacity()


	int HashTable_getCapacity(HashTable* self) {
		return self->capacity;
	} // getCapacity()


	/* static */ int HashTable_applyCapacityLimits(int capacity) {

		if (capacity < HashTable_CAPACITY_MIN) {
			capacity = HashTable_CAPACITY_MIN;
		}

		if (capacity > HashTable_CAPACITY_MAX) {
			capacity = HashTable_CAPACITY_MAX;
		}

		capacity = Utils_roundUp2Power(capacity);

		return capacity;
	} // applyCapacityLimits()


	void HashTable_calcMinCapacity(HashTable* self,int minCapacity) {

		// if no minimum set
		if (minCapacity == -1) minCapacity = self->minCapacity;
		if (minCapacity == -1) minCapacity = HashTable_CAPACITY_MIN;

		// apply limits
		minCapacity = HashTable_applyCapacityLimits(minCapacity);

		// it must be least 2x number of elements
		int optimalMin = HashTable_calcOptimalCapacity(self);
		if (optimalMin > minCapacity) minCapacity = optimalMin;

		// apply limits
		self->minCapacity = HashTable_applyCapacityLimits(minCapacity);

	} // calcMinCapacity()


	int HashTable_calcOptimalCapacity(HashTable* self) {

		int optimal;
		if (self->lastCommandEffect == HashTable_CAPACITY_EXPAND) {
			optimal = self->numberOfElms * 2;
		} else {
			optimal = self->numberOfElms * 3;
		}

		optimal = Utils_roundUp2Power(optimal);

		return optimal;
	} // calcOptimalCapacity()


	int HashTable_getNumberOfElms(HashTable* self) {
		return self->numberOfElms;
	} // getNumberOfElms()


	int HashTable_getCollisionPercent(HashTable* self) {

		if (self->numberOfElms == 0) return 0;

		int collision = 0;

		for (int i = 0; i < self->capacity; i++) {

			HashItem* item = self->items[i];
			while (item != NULL) {
				if (HashItem_getNext(item) != NULL) collision++;
				item = HashItem_getNext(item);
			} // loop()

		} 

		int result = (collision * 100) / self->numberOfElms;

		return result;
	} // getCollisionPercent()


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
			while (item != NULL) {
				HashItem_dump(item);
				item = HashItem_getNext(item);
			}
			printf("\n");
		} // foreach items

	} // dump()


	HashItem** HashTable_findItem(HashTable* self,int hash,char* data,int length) {

		HashItem** itemPtr = &self->items[hash];
		while (1) {

			if ((*itemPtr) == NULL) return NULL;
			
			if ( HashItem_getKeyLength(*itemPtr) == length ) {
				if (0 == memcmp(HashItem_getKeyData(*itemPtr),data,length)) return itemPtr;
			} // if length match

			itemPtr = HashItem_getNextPtr(*itemPtr);

		} // loop

		return NULL;
	} // findItem()


	inline void HashTable_linkItem(HashTable* self,HashItem* item,int hash) {

		HashItem_setNext(item,self->items[hash]);
		self->items[hash] = item;

		return;
	} // linkItem()


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
		HashItem_setReorgMark(item,self->reorgMark);
		if (-1 == HashItem_setKey(item,keydata,keylen)) HashTable_outOfMemory(self,2802);
		if (-1 == HashItem_setValue(item,valdata,vallen)) HashTable_outOfMemory(self,2802);

		// insert new item in slot (to the beginning of the linked list)
		HashTable_linkItem(self,item,hash);

		self->numberOfElms++;
		self->lastCommandEffect = HashTable_CAPACITY_EXPAND;
		HashTable_performReorg(self,-1);

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
		self->lastCommandEffect = HashTable_CAPACITY_SHRINK;
		HashTable_performReorg(self,-1);

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

		self->lastCommandEffect = HashTable_CAPACITY_SHRINK;
		HashTable_performReorg(self,-1);

		return HashTable_ZAP_ZAPPED;
	} // performZap()


	int HashTable_performSearch(HashTable* self,Search* search) {

		if (self->numberOfElms == 0) return HashTable_SEARCH_NOT_FOUND;

		Search_resetResults(search,self->numberOfElms);
		for (int i = 0; i < self->capacity; i++) {

			HashItem* item = self->items[i];
			while (item != NULL) {
				if ( Search_process(search,item) ) goto search_finished; // break 2
				item = HashItem_getNext(item);
			} // loop()

		} search_finished: // foreach slot
		
		if (Search_getNumberOfResults(search) == 0) return HashTable_SEARCH_NOT_FOUND;
		return HashTable_SEARCH_PROVIDED;
	} // performSearch()


	int HashTable_performReorg(HashTable* self,int method) {

		if (method == -1) method = self->method;

		int capacity = HashTable_calcOptimalCapacity(self);
		if (capacity < self->minCapacity) capacity = self->minCapacity;
		capacity = Utils_roundUp2Power(capacity);

		if ((method == self->method) && (capacity == self->capacity)) {
			return HashTable_REORG_UNCHANGED;
		} // if unchanged

		if (self->lastCommandEffect == HashTable_CAPACITY_SHRINK) {
			if (capacity > self->capacity) return HashTable_REORG_UNCHANGED;
		} else {
			if (capacity < self->capacity) return HashTable_REORG_UNCHANGED;
		}

		self->reorgMark ^= 0xff;
		self->method = method;
		self->hashMask = Utils_calcHashMask(capacity);	

		if (self->capacity < capacity) {
			HashTable_resizeMemory(self,capacity);
			for (int i = self->capacity; i < capacity; i++) self->items[i] = NULL;
		}
		HashTable_moveItems(self);
		if (self->capacity > capacity) HashTable_resizeMemory(self,capacity);

		self->capacity = capacity;

		return HashTable_REORG_PERFORMED;
	} // performReorg()


	void HashTable_resizeMemory(HashTable* self,int capacity) {

		self->items = (HashItem**)realloc(self->items,capacity * sizeof(HashItem*));
		if (self->items == NULL) HashTable_outOfMemory(self,2803);

	} // resizeMemory()


	void HashTable_moveItems(HashTable* self) {

		int processed = 0;
		for (int i = 0; i < self->capacity; i++) {

			HashItem** itemPtr = &self->items[i];
			while (*itemPtr != NULL) {
				HashItem* item = *itemPtr;

				// already moved
				if (HashItem_getReorgMark(item) == self->reorgMark) {
					itemPtr = HashItem_getNextPtr(item);
					continue;
				}

				// mark as moved
				HashItem_setReorgMark(item,self->reorgMark);

				// generate hash
				char* keyData = HashItem_getKeyData(item);
				int keyLength = HashItem_getKeyLength(item);
				int hash = HashTable_getHash(self,keyData,keyLength);					

				// new hash is same, don't move
				if (i == hash) {
					itemPtr = HashItem_getNextPtr(item);
				}

				// new hash is different, move item
				else {

					// save pointer-to-actual, it should point to next
					HashItem** prev = itemPtr;

					// get next (it's familiar, see above, before "continue" statements)
					itemPtr = HashItem_getNextPtr(item);

					// pointer-to-actual now points to the next, so actual is finally "in the air"
					*prev = *itemPtr;

					// link actual to its new place
					HashTable_linkItem(self,item,hash);

				} // else

				processed++;
				if (processed >= self->numberOfElms) return;
	
			} // loop()
		} // foreach slot

	} // moveItems()

