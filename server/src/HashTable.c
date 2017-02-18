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
		// NOP
	} // ctor


	void HashTable_dtor(HashTable* self) {
		// NOP
	} // dtor


	void HashTable_setLogger(HashTable* self,Logger* logger) {
		self->logger = logger;
	} // setLogger()


