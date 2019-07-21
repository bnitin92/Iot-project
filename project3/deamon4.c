//run using :
//gcc -o daemon daemon2.c
//./daemon


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <syslog.h>
#include<errno.h>
#include<string.h>

#define OK 0
#define ERR_FORK 2
#define ERR_CHDIR 8
#define ERR_WTF 7
#define ERR_SETSID 6



static void _signal_handler(const int signal)
{
  switch(signal) {
    case SIGHUP:
     break;
    case SIGTERM:
      syslog(LOG_INFO, "received SIGTERM and exiting");
      closelog();
      exit(OK);
      break;
    default:
      syslog(LOG_INFO, "receival of unhandled signal");
   }
 }

 int _do_work(void)
 {
 time_t current;
 struct tm *tm;
 while(1)
 {
  current =time(0);
  sleep (200);
 if((tm =localtime(&current))==NULL)
 {
 syslog(LOG_INFO,"Time to extract the error");
 return 1;
 }
  syslog(LOG_INFO,"%02d:%02d:%02d", tm->tm_hour,tm->tm_min,tm->tm_sec);

 }
 }





int main(void)

{

const char * ERROR_FORMAT="the error %m";

openlog("DAEMON_NAME", LOG_PID | LOG_NDELAY | LOG_NOWAIT, LOG_DAEMON);
syslog(LOG_INFO, "the starting is sampled");

pid_t pid = fork();

if (pid < 0) {
  syslog(LOG_ERR, ERROR_FORMAT, strerror(errno));
  return ERR_FORK;
}

if (pid > 0) {
   return OK;
}

if(setsid() < -1) {
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
