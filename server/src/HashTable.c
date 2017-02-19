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

		self->method = HashTable_METHOD_DEFAULT;
		self->capacity = Utils_roundUp2Power(HashTable_CAPACITY_DEFAULT);
		self->hashMask = Utils_calcHashMask(self->capacity);		
		self->numberOfElms = 0;

		self->items = (HashItem**)malloc(self->capacity * sizeof(HashItem*));
		if (self->items == NULL) {
			Logger_log(
				self->logger
				,Logger_FATAL
				,2801,"Out of memory"
			);
			exit(2);
		} // if out of memory

		for (int i = 0; i < self->capacity; i++) {
			self->items[i] = NULL;
		}

	} // ctor


	void HashTable_dtor(HashTable* self) {
		// NOP
	} // dtor


	void HashTable_setLogger(HashTable* self,Logger* logger) {
		self->logger = logger;
	} // setLogger()


	int HashTable_getMethod(HashTable* self) {
		return self->method;
	} // getMethod()


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

	
	int HashTable_performSet(HashTable* self,char* keydata,int keylen,char* valdata,int vallen) {


		int hash = Hasher_hash(self->method,keydata,keylen);

		return 1;
	} // performSet()

