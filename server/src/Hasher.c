#include "Hasher.h"


	int Hasher_hash(int method,char* key,int length) {

		if (length == 0) return Hasher_hash(method,"\0",1);

		switch (method) {
			case 1: return Hasher_hashDebug(key,length);
		}

		return 0;
	} // hash()


	int Hasher_hashDebug(char* key,int length) {

		int result = key[0];
		result |= key[0] << 8;
		result |= key[0] << 16;
		result |= key[0] << 24;

		return result;
	} // hashDebug()