#ifndef Utils_h_included
#define Utils_h_included

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>


// static class Utils

	// public static
	void Utils_getStamp(char* stamp);
	void Utils_setBufInt(unsigned char* buffer,int value);
	int Utils_getBufInt(const unsigned char* buffer);
	int Utils_isEqSigs(const char* sig1,const char* sig2);
	void Utils_dump(const char* buffer,int len);

#endif