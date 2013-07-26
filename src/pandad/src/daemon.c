/*
** Title:daemon.c
**
** Description:
**   Driver for mellow panda daemon.
**
**   Normal use case is as a background daemon.
**   Will execute in foreground for testing purposes.
**
**   See documentation for command line details.
**
** Development Environment:
**   Ubuntu 8.10
**   gcc version 4.3.2 (Ubuntu 4.3.2-1ubuntu12)
**
** Legalese:  
**   Copyright (C) 2010 Digital Burro, INC.
**
** Author:
**   G.S. Cole (guycole at gmail dot com)
**
** Maintenance History:
**   $Log$
*/
static char rcsid[] = "$Id$";

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "command.h"
#include "configuration.h"
#include "daemon.h"
#include "device.h"
#include "eclectic.h"
#include "log.h"
#include "socket.h"

extern CONFIGURATION gConfiguration;

static BOOLEAN gRunFlag;
static void signal_handler(int signal2);

/*
** 
*/
void write_diagnostics(void) {
  char buffer[MAX_RESPONSE_BUFFER];

  configuration_report(buffer);
//  printf("%s\n", buffer);
}

/*
** ensure there is only one active copy of pandad
*/
void pid_lockx() {
  int rc;
  char buffer[32];
  char log_message[MAX_LOG_MESSAGE];

  sprintf(log_message, "pid lock file:%s:%d", PID_LOCK_FILE_NAME, getpid());
  log_debug(__FILE__, __LINE__, log_message);

  gConfiguration.pid_lock_fd = open(PID_LOCK_FILE_NAME, O_RDWR|O_CREAT, 0600);
  if (gConfiguration.pid_lock_fd < 0) {
    perror("lock file failure1");
    fprintf(stderr, "lock file open failure");
    log_fatal(__FILE__, __LINE__, "lock file open failure");
  }

  if (lockf(gConfiguration.pid_lock_fd, F_TLOCK, 0) < 0) {
    perror("lock file failure2");
    fprintf(stderr, "lock file failure");
    log_fatal(__FILE__, __LINE__, "lock file failure");
  }

  sprintf(buffer, "%ld\n", (long) getpid());

  rc = write(gConfiguration.pid_lock_fd, buffer, strlen(buffer));
}

/*
** HUP rereads configuration
** TERM causes graceful exit
*/
void establish_signal_handlers(void) {
  sigset_t action_set;
  struct sigaction action;

  sigemptyset(&action_set);
  sigaddset(&action_set, SIGHUP);
  sigaddset(&action_set, SIGTERM);

  action.sa_handler = signal_handler;
  action.sa_flags =  SA_RESETHAND|SA_NODEFER;

  if (sigaction(SIGHUP, &action, NULL)) {
    log_fatal(__FILE__, __LINE__, "sigaction() failure for HUP");
  }

  if (sigaction(SIGTERM, &action, NULL)) {
    log_fatal(__FILE__, __LINE__, "sigaction() failure for TERM");
  }
}

/*
** enter daemon mode
*/
void establish_daemon(void) {
  pid_t pid;
  int fd, ndx, retstat;

  struct rlimit resource_limit;
  char log_message[MAX_LOG_MESSAGE];

  /*
  ** first fork, close controlling terminal, change session group
  */

  pid = fork();
  switch(pid) {
  case -1:
    log_fatal(__FILE__, __LINE__, "fork failure//1");
  case 0:
    sprintf(log_message, "fork//1 w/new pid:%d", getpid());
    log_debug(__FILE__, __LINE__, log_message);
    break;
  default:
    exit(EXIT_SUCCESS);
  }

  //close all file descriptors
  resource_limit.rlim_cur = 0;
  resource_limit.rlim_max = 0;

  retstat = getrlimit(RLIMIT_NOFILE, &resource_limit);
  if (retstat < 0) {
    log_fatal(__FILE__, __LINE__, "getrlimit failure");
  }

  for(ndx = 0; ndx < resource_limit.rlim_max; ndx++) {
    close(ndx);
  }

  retstat = setsid();
  if (retstat < 0) {
    log_fatal(__FILE__, __LINE__, "setsid failure");
  }

  /*
  ** second fork
  ** create session and process group without controlling terminal
  */
  pid = fork();
  switch(pid) {
  case -1:
    log_fatal(__FILE__, __LINE__, "fork failure//2");
  case 0:
    sprintf(log_message, "fork//2 w/new pid:%d", getpid());
    log_debug(__FILE__, __LINE__, log_message);
    break;
  default:
    exit(EXIT_SUCCESS);
  }

  if (chdir("/") < 0) {
    log_fatal(__FILE__, __LINE__, "chdir failure");
  }

  // new PID, new lock...
  pid_lockx();

  umask(0);

  fd = open("/dev/null", O_RDWR); //stdin

  if (dup(fd) < 0) { //stdout
    log_fatal(__FILE__, __LINE__, "dup failure//1");
  }

  if (dup(fd) < 0) {  //stderr
    log_fatal(__FILE__, __LINE__, "dup failure//2");
  }
}

/*
** run until SIGTERM noted
*/
void run_loop(void) {
  if (!gConfiguration.foreground_mode) {
    establish_daemon();
  } else {
    pid_lockx();
    printf("foreground mode\n");
  }

  gConfiguration.command_fd = socket_server();

  device_open_all();

  if (gConfiguration.experiment != NULL) {
    eclectic_test(gConfiguration.experiment);
    return;
  }

  //
  // fall into main loop
  //  poll command socket and RS-232/serial ports for work
  //  exit when SIGTERM noted
  //
  gRunFlag = TRUE;
  while(gRunFlag) {
    eclectic();
  }
}

/*
** application stop invoked by TERM signal
*/
void shut_down(void) {
  log_info(__FILE__, __LINE__, "shut_down");
  log_shutdown();

  close(gConfiguration.command_fd);
  gConfiguration.command_fd = 0;

  //  close(gConfiguration.pid_lock_fd);
  //  gConfiguration.pid_lock_fd = -1;

  device_close_all();
}

/*
** application start
*/
BOOLEAN start_up(char *hostname, int port) {
  gConfiguration.boot_time = time(NULL);

  if (gConfiguration.write_version) {
    char temp1[64];
    pandad_version_string(temp1);
    fprintf(stderr, "%s\n", temp1); 
    return(FALSE);
  }

  log_setup(DAEMON_LOG_ID);
  log_info(__FILE__, __LINE__, "start_up");

  configuration_read();

  if (hostname != NULL) {
    free(gConfiguration.server_name);
    gConfiguration.server_name = hostname;
  }

  if (port > 0) {
    gConfiguration.command_port = port;
  }

  if (gConfiguration.diagnostic_mode) {
    write_diagnostics();
    return(FALSE);
  }

  establish_signal_handlers();

  return(TRUE);
}

/*
** operator commands restart
*/
static void signal_handler(int signal2) {
  char log_message[MAX_LOG_MESSAGE];

  switch(signal2) {
  case SIGHUP:
    log_info(__FILE__, __LINE__, "HUP signal received; restart requested");
    shut_down();
    configuration_defaults();
    start_up(NULL, -1);
    break;
  case SIGTERM:
    log_info(__FILE__, __LINE__, "TERM signal received; shutdown requested");
    gRunFlag = FALSE;
    break;
  default:
    sprintf(log_message, "unknown signal:%d", signal2);
    log_error(__FILE__, __LINE__, log_message);
  }

  establish_signal_handlers();
}

/*
** main entry point for pandad
*/
int main(int argc, char *argv[]) {
  int opt;
  int port = -1;
  char *hostname = NULL;

  bzero(&gConfiguration, sizeof(CONFIGURATION));

  configuration_defaults();

  while ((opt = getopt(argc, argv, "c:dfh:p:x:V")) != -1) {
    switch(opt) {
    case 'c':
      fprintf(stderr, "non-standard configuration file\n");
      gConfiguration.configuration_file_name = strdup(optarg);
      break;
    case 'd':
      fprintf(stderr, "dump configuration\n");
      gConfiguration.diagnostic_mode = TRUE;
      break;
    case 'f':
      fprintf(stderr, "foreground mode\n");
      gConfiguration.foreground_mode = TRUE;
      break;
    case 'h':
      fprintf(stderr, "non-standard host\n");
      hostname = strdup(optarg);
      break;
    case 'p':
      fprintf(stderr, "non-standard client port\n");
      port = atoi(optarg);
      break;
    case 'x':
      fprintf(stderr, "experimental command test\n");
      gConfiguration.experiment = strdup(optarg);
      gConfiguration.foreground_mode = TRUE;
      break;
    case 'V':
      //write program version and exit
      gConfiguration.write_version = TRUE;
      break;
    default:
      fprintf(stderr, "usage:%s [-c file][-d][-f][-h host][-p port][-x experiment][-V]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (start_up(hostname, port)) {
    run_loop();
  }

  shut_down();

  exit(EXIT_SUCCESS);
}
