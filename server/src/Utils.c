#include "Utils.h"

	void Utils_fatal(const char* message) {

		fprintf(stderr,"%s \n",message);
		exit(1);

	} // fatal()
