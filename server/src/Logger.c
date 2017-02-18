#include "Logger.h"



	Logger* new_Logger() {

		Logger* self;
		self = (Logger*)malloc(sizeof(Logger));
		if (self != NULL) Logger_ctor(self);

		return self;
	} // new 


	void delete_Logger(Logger* self) {

		Logger_dtor(self);
		free(self);

	} // delete 


	void Logger_ctor(Logger* self) {

		self->level = Logger_ERROR;
		self->filename = "error.log";
		self->lastId = 0;

	} // ctor


	void Logger_dtor(Logger* self) {

		// NOP

	} // dtor


	int Logger_getLastId(Logger* self) {
		return self->lastId;
	} // getLastId()


	void Logger_setFilename(Logger* self,const char* fnam) {
		self->filename = fnam;
	} // setFilename()


	void Logger_setLevel(Logger* self,int value) {
		self->level = value;
	} // setLevel()


	const char* Logger_getLevelString(int level) {
		
		level = level & 0x0f;

		if (level == Logger_DEBUG) return "DEBUG";
		if (level == Logger_NOTICE) return "NOTICE";
		if (level == Logger_ERROR) return "ERROR";
		if (level == Logger_FATAL) return "FATAL";

		return "-";
	} // getLevelString()


	void Logger_logi(Logger* self,int level,int id,const char* message,int value) {

		char buffer[200];
		snprintf(buffer,200,message,value);
		Logger_log(self,level,id,buffer);

	} // logi()


	void Logger_log(Logger* self,int level,int id,const char* message) {

		self->lastId = id;

		if ((self->level & 0x0f) == Logger_MUTE) {
			level &= (~Logger_DISPLAY);
		}
		if (level & Logger_DISPLAY) {
			fprintf(stderr,"\r%s \n",message);
		}
		if ((level &  0x0f) < self->level) return;

		FILE* file = fopen(self->filename,"a+");
		if (file == NULL) return;

		char stamp[80];
		Utils_getStamp(stamp);

		fprintf(
			file
			,"%s [%s] %d: %s \n"
			,stamp
			,Logger_getLevelString(level)
			,id
			,message
		);

		fclose(file);

	} // log()