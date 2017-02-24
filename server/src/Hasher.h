#ifndef Hasher_h_included
#define Hasher_h_included


#define Hasher_METHOD_DEBUG 1
#define Hasher_METHOD_MAX 10
#define Hasher_METHOD_DEFAULT 10


// static
	int Hasher_hash(int method,char* key,int length);
	char* Hasher_getName(int token);
	int Hasher_debug(int pos,char* key,int length);
	int Hasher_additive(unsigned char* p,int len);
	int Hasher_xor(unsigned char* p,int len);
	int Hasher_rotating(unsigned char* p,int len);
	int Hasher_bernstein(unsigned char* p,int len);
	int Hasher_modifiedBernstein(unsigned char* p,int len);
	int Hasher_shiftAddXor(unsigned char* p,int len);
	int Hasher_fowlerNollVo(unsigned char* p,int len);
	int Hasher_oneAtATime(unsigned char* p,int len);
	int Hasher_elf(unsigned char* p,int len);

#endif