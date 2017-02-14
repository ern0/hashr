#include "Utils.h"


	void Utils_getStamp(char* stamp) {

		char fmt[64];
		char buf[64];
		struct timeval tv;
		struct tm* tm;

		gettimeofday(&tv,NULL);
		tm = localtime(&tv.tv_sec);
		strftime(fmt,sizeof(fmt),"%Y-%m-%d %H:%M:%S",tm);
		sprintf(stamp,fmt,tv.tv_usec);

	} // getStamp()
