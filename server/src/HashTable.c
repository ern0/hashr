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
		self->capacity = HashTable_CAPACITY_DEFAULT;
		self->numberOfElms = 0;

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
