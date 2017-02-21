#ifndef Logger_h_included
#define Logger_h_included

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "Utils.h"


// class Logger

	// consts
	#define Logger_DEBUG 1
	#define Logger_NOTICE 2
	#define Logger_ERROR 3
	#define Logger_FATAL 4
	#define Logger_MUTE 5  /* only for testing */
	#define Logger_DISPLAY 0x80
	#define Logger_LEVEL_MASK 0x0f

	// private
	struct Logger {
		const char* filename;
		int level;
		int lastId;
	};
	typedef struct Logger Logger;


	// public
	Logger* new_Logger();
	void delete_Logger(Logger* self);
	void Logger_setFilename(Logger* self, const char* fnam);
	void Logger_setLevel(Logger* self,int value);
	void Logger_log(Logger* self,int level,int id,const char* message);
	void Logger_logi(Logger* self,int level,int id,const char* message,int value);
	int Logger_getLastId(Logger* self);

	// protected
	void Logger_ctor(Logger* self);
	void Logger_dtor(Logger* self);
	/* static */ const char* Logger_getLevelString(int level);


#endif