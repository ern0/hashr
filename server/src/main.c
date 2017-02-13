#include "Server.h"


	int main(int argc,char* argv[]) {

		Server server; Server_ctor(&server);
		Server_setPort(&server,8000);
		Server_prepare(&server);
		fprintf(stderr,"Server is listening on port %d... \n",Server_getPort(&server));

		Server_run(&server);  // runs forever

		Server_dtor(&server);

	} // main()
