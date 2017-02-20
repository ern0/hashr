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
		
		self->next = NULL;

		self->keyData = NULL;
		self->keyLength = 0;
		self->valueData = NULL;
		self->valueLength = 0;

		self->method = -1;

	} // ctor


	void HashItem_dtor(HashItem* self) {

		if (self->keyData != NULL) free(self->keyData);
		if (self->valueData != NULL) free(self->valueData);

	} // dtor


	void HashItem_setMethod(HashItem* self,int method) {
		self->method = method;
	} // setMethod()


	void HashItem_dump(HashItem* self) {
		printf(
			"%d %c%c%c%c %c%c%c%c"
			,self->method
			,self->keyData[0],self->keyData[1],self->keyData[2],self->keyData[3]
			,self->valueData[0],self->valueData[1],self->valueData[2],self->valueData[3]
		);
	} // dump()


	int HashItem_setKey(HashItem* self,char* data,int length) {

		self->keyData = malloc(length);
		if (self->keyData == NULL) return -1;

		memcpy(self->keyData,data,length);
		self->keyLength = length;

		return 0;
	} // setKey()


	int HashItem_setValue(HashItem* self,char* data,int length) {

		self->valueData = malloc(length);
		if (self->valueData == NULL) return -1;

		memcpy(self->valueData,data,length);
		self->valueLength = length;

		return 0;
	} // setValue()


	int HashItem_replaceValue(HashItem* self,char* data,int length) {

		if (self->valueLength != length) {
			self->valueData = realloc(self->valueData,length);
			if (self->valueData == NULL) return -1;
		}

		memcpy(self->valueData,data,length);
		self->valueLength = length;

		return 0;
	} // replaceValue()