#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <getopt.h>
#include "Utils.h"
#include "Logger.h"
#include "Server.h"

	
	Server* server; 
	Logger* logger;
	

	void signalHandler(int signum) {
		Server_stop(server);
	} // signalHandler()


	void savePidFile(char* fnam) {

		FILE* f = fopen(fnam,"wt");
		if (f == NULL) return;
		fprintf(f,"%d",getpid());
		fclose(f);

	} // savePidFile()


	void daemonize() {

		pid_t pid = fork();
		if (pid == -1) exit(4);
		if (pid != 0) exit(0);
		
		if (setsid() == -1) exit(4);
		if (chdir("/")) exit(4);
		
		for (int i = 0; i < 12; i++) close(i);
		if (-1 == open("/dev/null",O_RDWR)) exit(4);
		int avoidWarning = dup(0) | dup(0);

	} // daemonize()


	int main(int argc,char* argv[]) {

		int port = 0;
		int daemonMode = 0;
		char* logFile = NULL;
		char* pidFile = NULL;

		int opt;
		while ((opt = getopt(argc,argv,"?p:dl:f:")) != -1) {
			switch (opt) {
				case 'p':
					port = atoi(optarg);
					break;
				case 'd':
					daemonMode = 1;
					break;
				case 'l':
					logFile = optarg;
					break;
				case 'f':
					pidFile = optarg;
					break;
				default:
					fprintf(stderr,"Usage: \n");
					fprintf(stderr," -p <port>: set listen port, default: 8000 \n");
					fprintf(stderr," -d: daemon mode\n");
					fprintf(stderr," -l <fnam>: logfile, default: /tmp/hashr.log \n");
					fprintf(stderr," -f <fnam>: pidfile, default: no pidfile \n");
					fprintf(stderr,"    note: use full paths in daemon mode\n");
					exit(1);
			} // switch opt
		} // wile getopt()

		if (daemonMode) daemonize();

		if (port == 0) port = 8000;
		if (logFile == NULL) logFile = "/tmp/hashr.log";
		if (pidFile != NULL) savePidFile(pidFile);

		logger = new_Logger();
		Logger_setFilename(logger,logFile);
		Logger_setLevel(logger,Logger_DEBUG);

		Logger_log(logger,Logger_DEBUG,9001,"application started");

		server = new_Server();
		if (server == NULL) {
			Logger_log(logger,Logger_ERROR,9002,"out of memory");
			exit(2);
		}

		Server_setLogger(server,logger);
		Server_setPort(server,port);
		Server_start(server);
		Logger_logi(
			logger
			,Logger_NOTICE | Logger_DISPLAY
			,9003,"server is listening on port %d..."
			,Server_getPort(server)
		);

		signal(SIGINT,signalHandler);
		signal(SIGTERM,signalHandler);
		if (daemonMode) {
			signal(SIGHUP,SIG_IGN);
		} else {
			signal(SIGHUP,signalHandler);
		}

		Server_run(server);

		Logger_log(
			logger
			,Logger_NOTICE | Logger_DISPLAY
			,9004,"exiting..."
		);
		delete_Server(server);
		delete_Logger(logger);

		Logger_log(logger,Logger_DEBUG,9005,"application finished normally");

	} // main()