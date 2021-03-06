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


	int Utils_isEqSigs(const char* sig1,const char* sig2) {

		if (sig1[0] != sig2[0]) return 0;
		if (sig1[1] != sig2[1]) return 0;
		if (sig1[2] != sig2[2]) return 0;
		if (sig1[3] != sig2[3]) return 0;

		return 1;
	} // isEqSigs()


	void Utils_setBufInt(unsigned char* buffer,int value) {

		buffer[0] = (value >> 24) & 0xff;
		buffer[1] = (value >> 16) & 0xff;
		buffer[2] = (value >> 8) & 0xff;
		buffer[3] = value & 0xff;
		
	} // setBufInt()


	int Utils_getBufInt(const unsigned char* buffer) {
		return buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];
	} // getBufInt()


	void Utils_dump(const char* buffer,int len) {

		int ptr = 0;

		while (1) {
			if (ptr >= len) break;

			printf("  %04X: ",ptr);

			for (int i = 0; i < 8; i++) {
				int index = ptr + i;

				if (index < len) {
					char b = buffer[ptr + i];
					printf("%02X ",b);
				} // if payload

				else {
					printf("   ");
				} // else fill

			} // for numbers

			printf(" \e[7m");

			for (int i = 0; i < 8; i++) {
				int index = ptr + i;

				if (index < len) {
					char b = buffer[ptr + i];
					if (b < ' ') b = '.';
					if (b > 126) b = '.';
					printf("%c",b);
				} // if payload

			} // for characters

			printf("\e[0m\n");

			ptr += 8;
		} // while buffer

	} // dumpBuffer()


	int Utils_roundUp2Power(int value) {

		--value;

		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;

		value++;

		return value;
	} // roundUp2Power()


	int Utils_calcHashMask(int value) {

		--value;

		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;

		return value;
	} // calcHashMask()
