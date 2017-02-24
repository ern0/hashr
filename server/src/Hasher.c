#include "Hasher.h"


	int Hasher_hash(int method,char* key,int length) {

		if (length == 0) return Hasher_hash(method,(char*)0,1);
		unsigned char* p = (unsigned char*)key;

		switch (method) {
			case 0: return Hasher_debug(0,key,length);
			case 1: return Hasher_debug(1,key,length);
			case 2: return Hasher_additive(p,length);
			case 3: return Hasher_xor(p,length);
			case 4: return Hasher_rotating(p,length);
			case 5: return Hasher_bernstein(p,length);
			case 6: return Hasher_modifiedBernstein(p,length);
			case 7: return Hasher_shiftAddXor(p,length);
			case 8: return Hasher_fowlerNollVo(p,length);
			case 9: return Hasher_oneAtATime(p,length);
			case 10: return Hasher_elf(p,length);
		}

		return 0;
	} // hash()


	char* Hasher_getName(int token) {
		
		switch (token) {
			case 0: return "debug0";
			case 1: return "debug1";
			case 2: return "additive";
			case 3: return "xor";
			case 4: return "rotating";
			case 5: return "Bernstein";
			case 6: return "modified Bernstein";
			case 7: return "shift-add-xor";
			case 8: return "Fowler-Noll-Vo";
			case 9: return "one-at-a-time";
			case 10: return "elf";
			default: return "(n.a.)";
		}

	} // getName()


	int Hasher_debug(int pos,char* key,int length) {

		if (length <= pos) return 0;

		int result = key[pos];
		result |= key[0] << 8;
		result |= key[0] << 16;
		result |= key[0] << 24;

		return result;
	} // debug()


	// The following hashing methods are harvested from this page:
	//   http://www.eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx


	int Hasher_additive(unsigned char* p,int len) {
		int h = 0;

		for (int i = 0; i < len; i++) {
			h += p[i];
		}

		return h;
	} 


	int Hasher_xor(unsigned char* p,int len) {
		int h = 0;

		for (int i = 0; i < len; i++) {
			h ^= p[i];
		}

		return h;
	} 


	int Hasher_rotating(unsigned char* p,int len) {
		int h = 0;

		for (int i = 0; i < len; i++) {
			h = (h << 4) ^ (h >> 28) ^ p[i];
		}

		return h;
	} 


	int Hasher_bernstein(unsigned char* p,int len) {
		int h = 0;

		for (int i = 0; i < len; i++) {
    	h = 33 * h + p[i];		
  	}

		return h;
	} 


	int Hasher_modifiedBernstein(unsigned char* p,int len) {
		int h = 0;

		for (int i = 0; i < len; i++) {
			h = 33 * h ^ p[i];
  	}

		return h;
	}


	int Hasher_shiftAddXor(unsigned char* p,int len) {
		int h = 0;

		for (int i = 0; i < len; i++) {
			h ^= (h << 5) + (h >> 2) + p[i];
		}

		return h;
	}


	int Hasher_fowlerNollVo(unsigned char* p,int len) {

		unsigned int h = 2166136261;

		for (int i = 0; i < len; i++) {
			h = (h * 16777619) ^ p[i];
		}

		return (int)h;
	}


	int Hasher_oneAtATime(unsigned char* p,int len) {
		unsigned int h = 0;

		for (int i = 0; i < len; i++) {
			h += p[i];
			h += (h << 10);
			h ^= (h >> 6);
		}

		h += (h << 3);
		h ^= (h >> 11);
		h += (h << 15);

		return (int)h;
	}


	int Hasher_elf(unsigned char* p,int len) {
		unsigned int h = 0;
		unsigned int g;

		for (int i = 0; i < len; i++) {

			h = (h << 4) + p[i];
			g = h & 0xf0000000L;

			if (g != 0) {
				h ^= g >> 24;
			}

			h &= ~g;
		}

		return (int)h;
	}

