#include "HashItem.h"


	HashItem* new_HashItem() {

		HashItem* self;
		self = (HashItem*)malloc(sizeof(HashItem));
		if (self != NULL) HashItem_ctor(self);

		return self;
	} // new


	void delete_HashItem(HashItem* self) {

		HashItem_dtor(self);
		free(self);

	} // delete


	void HashItem_ctor(HashItem* self) {
		// NOP
	} // ctor


	void HashItem_dtor(HashItem* self) {
		// NOP
	} // dtor


	void HashItem_dump(HashItem* self) {
		printf(
			"%d %c%c%c%c %c%c%c%c"
			,self->method
			,self->keyData[0],self->keyData[1],self->keyData[2],self->keyData[3]
			,self->valueData[0],self->valueData[1],self->valueData[2],self->valueData[3]
		);
	} // dump()


	void HashItem_setKey(HashItem* self,char* data,int length) {
		self->keyData = data;
		self->keyLength = length;
	} // setKey()


	void HashItem_setValue(HashItem* self,char* data,int length) {
		self->valueData = data;
		self->valueLength = length;
	} // setValue()
