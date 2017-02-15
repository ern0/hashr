#include <signal.h>
#include "Utils.h"
#include "Logger.h"
#include "Server.h"

	
	Server* server; 
	Logger* logger;
	

	void signalHandler(int signum) {
		Server_stop(server);
	} // signalHandler()


	int main(int argc,char* argv[]) {

		logger = new_Logger();
		Logger_setFilename(logger,"hashr.log");
		Logger_setLevel(logger,Logger_DEBUG);

		Logger_log(logger,Logger_DEBUG,9001,"Application started");


		server = new_Server();
		if (server == NULL) {
			Logger_log(logger,Logger_ERROR,9002,"Out of memory");
			exit(2);
		}

		Server_setLogger(server,logger);
		Server_setPort(server,8000);
		Server_start(server);
		Logger_logi(
			logger
			,Logger_NOTICE | Logger_DISPLAY
			,9003,"Server is listening on port %d..."
			,Server_getPort(server)
		);

		signal(SIGINT,signalHandler);
		signal(SIGHUP,signalHandler);

		Server_run(server);

		Logger_log(
			logger
			,Logger_NOTICE | Logger_DISPLAY
			,9004,"Exiting..."
		);
		delete_Server(server);
		delete_Logger(logger);

		Logger_log(logger,Logger_DEBUG,9005,"Application finished normally");

	} // main()
