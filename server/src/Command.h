#ifndef Command_h_included
#define Command_h_included

#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Utils.h"
#include "Logger.h"

// class Packet;
struct Packet;
typedef struct Packet Packet;


// class Command

	// private
	struct Command {
		Packet* packet;
	};
	typedef struct Command Command;


	// public
	Command* new_Command();
	void delete_Command(Command* self);
	void Command_setPacket(Command* cmd,Packet* packet);
	void Command_processUnknown(Command* cmd);
	void Command_processInfo(Command* cmd);
	void Command_processSet(Command* cmd);
	void Command_processGet(Command* cmd);
	void Command_processDel(Command* cmd);
	void Command_processZap(Command* cmd);
	void Command_processKsearch(Command* cmd);
	void Command_processVsearch(Command* cmd);
	void Command_processSearch(Command* cmd);
	void Command_processReorg(Command* cmd);

	// protected
	void Command_ctor(Command* self);
	void Command_dtor(Command* self);


#endif