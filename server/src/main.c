#include <signal.h>
#include "Utils.h"
#include "Server.h"

	
	Server* server; 
	

	void signalHandler(int signum) {
		Server_stop(server);
	} // signalHandler()


	int main(int argc,char* argv[]) {

		server = new_Server();
		if (server == NULL) Utils_fatal("out of memory");

		Server_setPort(server,8000);
		Server_start(server);
		fprintf(stderr,"Server is listening on port %d... \n",Server_getPort(server));

		signal(SIGINT,signalHandler);
		signal(SIGHUP,signalHandler);

		Server_run(server);

		fprintf(stderr,"- exiting... \n");
		delete_Server(server);

	} // main()
