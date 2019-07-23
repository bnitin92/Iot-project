/* 
[x86]
gcc -c deamon_hw.c -o simpled.o
gcc -o simpled simpled.o
Check syslog: tail /var/log/syslog

[arm]
${BUILDROOT_HOME}/output/host/usr/bin/arm-linux-gcc --sysroot=${BUILDROOT_HOME}/output/staging  -c deamon_hw.c -o simpled.o
${BUILDROOT_HOME}/output/host/usr/bin/arm-linux-gcc --sysroot=${BUILDROOT_HOME}/output/staging  -o simpled simpled.o
Check messages: tail /var/log/messages

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <syslog.h>
#include <errno.h>
#include <string.h>

#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>

#define OK 0
#define ERR_FORK 2
#define ERR_CHDIR 8
#define ERR_WTF 7
#define ERR_SETSID 6

#define DAEMON_NAME "simpled"


static void _signal_handler(const int signal){
	switch(signal){
		case SIGHUP:
			break;
		case SIGTERM:
			syslog(LOG_INFO, "received SIGTERM, exiting.");
			closelog();
			exit(OK);
			break;
		default:
		syslog(LOG_INFO, "received unhandled signal");
	}
}

static void _do_work(void) {

	for(int i=0; true; i++) {

		time_t rawtime;
   		struct tm *info;
   		char buffer[80];

   		time( &rawtime );

   		info = localtime( &rawtime );

   		strftime(buffer,80,"%x - %H:%M:%S", info);

		syslog(LOG_INFO, "system time is : |%s|", buffer);
		sleep(1);
	}
}


int main(void) {

	const char * ERROR_FORMAT="the error %m";

	openlog(DAEMON_NAME, LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_DAEMON);
	syslog(LOG_INFO, "starting sampled");


	pid_t pid = fork();

	if (pid < 0){
		syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
		return ERR_FORK;
	}
	
	if (pid > 0) {
		return OK;
	}

	if(setsid() < -1){
		syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
		return ERR_SETSID;
	}

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	umask(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

	if (chdir("/") < 0) {
		syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
		return ERR_CHDIR;
	}

	signal(SIGTERM, _signal_handler);
	signal(SIGHUP, _signal_handler);

	_do_work();
 
 	return ERR_WTF;

}