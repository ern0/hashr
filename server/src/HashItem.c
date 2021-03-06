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

		self->reorgMark = 0;

	} // ctor


	void HashItem_dtor(HashItem* self) {

		if (self->keyData != NULL) free(self->keyData);
		if (self->valueData != NULL) free(self->valueData);

	} // dtor


	void HashItem_setReorgMark(HashItem* self,char reorgMark) {
		self->reorgMark = reorgMark;
	} // setReorgMark()


	char HashItem_getReorgMark(HashItem* self) {
		return self->reorgMark;
	} // getReorgMark()


	void HashItem_dump(HashItem* self) {
		
		if (self == NULL) {
			printf("(null)");
			return;
		}

		for (int i = 0; i < self->keyLength; i++) {
			printf("%c",self->keyData[i]);
		}

		printf("=");

		for (int i = 0; i < self->valueLength; i++) {
			printf("%c",self->valueData[i]);
		}

		printf(" ");

	} // dump()


	int HashItem_setKey(HashItem* self,char* data,int length) {

		self->keyData = malloc(length);
		if (self->keyData == NULL) return -1;

		memcpy(self->keyData,data,length);
		self->keyLength = length;

		return 0;
	} // setKey()


	char* HashItem_getKeyData(HashItem* self) {
		return self->keyData;
	} // getKeyData()


	int HashItem_getKeyLength(HashItem* self) {
		return self->keyLength;
	} // getKeyLength()


	int HashItem_setValue(HashItem* self,char* data,int length) {

		self->valueData = malloc(length);
		if (self->valueData == NULL) return -1;

		memcpy(self->valueData,data,length);
		self->valueLength = length;

		return 0;
	} // setValue()


	char* HashItem_getValueData(HashItem* self) {
		return self->valueData;
	} // getValueData()


	int HashItem_getValueLength(HashItem* self) {
		return self->valueLength;
	} // getValueLength()


	HashItem** HashItem_getNextPtr(HashItem* self) {
		return &self->next;  // yes, pointer to self->next
	} // getNextPtr()


	HashItem* HashItem_getNext(HashItem* self) {
		return self->next;
	} // getNext()


	void HashItem_setNext(HashItem* self,HashItem* next) {
		self->next = next;
	} // setNext()


	int HashItem_replaceValue(HashItem* self,char* data,int length) {

		if (self->valueLength != length) {
			self->valueData = realloc(self->valueData,length);
			if (self->valueData == NULL) return -1;
		}

		memcpy(self->valueData,data,length);
		self->valueLength = length;

		return 0;
	} // replaceValue()


	int HashItem_searchKey(HashItem* self,char* data,int length) {
		return (NULL != memmem(self->keyData,self->keyLength,data,length));
	} // searchKey()


	int HashItem_searchValue(HashItem* self,char* data,int length) {
		return (NULL != memmem(self->valueData,self->valueLength,data,length));
	} // searchValue()


	int HashItem_matchKey(HashItem* self,char* data,int length) {
		if (length != self->keyLength) return 0;
		return (0 == memcmp(self->keyData,data,length));
	} // matchKey()


	int HashItem_matchValue(HashItem* self,char* data,int length) {
		if (length != self->valueLength) return 0;
		return (0 == memcmp(self->valueData,data,length));
	} // matchValue()
