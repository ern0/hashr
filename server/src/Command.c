#include "Command.h"
#include "Packet.h"
#include "Logger.h"


	Command* new_Command() {

		Command* self;
		self = (Command*)malloc(sizeof(Command));
		if (self != NULL) Command_ctor(self);

		return self;
	} // new 


	void delete_Command(Command* self) {
		Command_dtor(self);
		free(self);
	} // delete 


	void Command_ctor(Command* self) {
		// NOP
	} // ctor


	void Command_dtor(Command* self) {
		// NOP
	} // dtor


	void Command_setPacket(Command* self,Packet* packet) {
		self->packet = packet;
	} // setPacket()


	void Command_processUnknown(Command* self) {
		printf("todo: unknown cmd \n");
	}


	void Command_processInfo(Command* self) {
		Packet_log(self->packet,Logger_NOTICE | Logger_DISPLAY,2201,"todo: implement info command");
	}


	void Command_processSet(Command* self) {
		printf("todo: set cmd \n");
	}


	void Command_processGet(Command* self) {
		printf("todo: get cmd \n");
	}

	
	void Command_processDel(Command* self) {
		printf("todo: del cmd \n");
	}

	
	void Command_processZap(Command* self) {
		printf("todo: zap cmd \n");
	}
	

	void Command_processKsearch(Command* self) {
		printf("todo: ksearch cmd \n");
	}


	void Command_processVsearch(Command* self) {
		printf("todo: vsearch cmd \n");
	}


	void Command_processSearch(Command* self) {
		printf("todo: search cmd \n");
	}


	void Command_processReorg(Command* self) {
		printf("todo: reorg cmd \n");
	}


