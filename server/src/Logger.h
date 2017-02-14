#ifndef Logger_h_included
#define Logger_h_included

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "Utils.h"


// class Logger

	// consts
	#define Logger_DEBUG 0x01
	#define Logger_NOTICE 0x02
	#define Logger_ERROR 0x03
	#define Logger_FATAL 0x04
	#define Logger_DISPLAY 0x80

	// private
	struct Logger {
		const char* filename;
		int level;
	};
	typedef struct Logger Logger;


	// public
	Logger* new_Logger();
	void delete_Logger(Logger* self);
	void Logger_setFilename(Logger* self, const char* fnam);
	void Logger_setLevel(Logger* self,int value);
	void Logger_log(Logger* self,int level,int id,const char* message);
	void Logger_logi(Logger* self,int level,int id,const char* message,int value);

	// protected
	void Logger_ctor(Logger* self);
	void Logger_dtor(Logger* self);
	/* static */ const char* Logger_getLevelString(int level);


#endif