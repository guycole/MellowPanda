/*
** Title:log.c
**
** Description:
**   Wrapper for syslogd(8)
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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "log.h"

/*
** debug level message
*/
void log_debug(const char *file_name, int line, const char *message) {
  assert(message != (char *) NULL);
  syslog(LOG_DEBUG, "%s %d %s", file_name, line, message);
}

/*
** informational level message
*/
void log_info(const char *file_name, int line, const char *message) {
  assert(message != (char *) NULL);
  syslog(LOG_INFO, "%s %d %s", file_name, line, message);
}

/*
** error level message
*/
void log_error(const char *file_name, int line, const char *message) {
  assert(message != (char *) NULL);
  syslog(LOG_ERR, "%s %d %s", file_name, line, message);
}

/*
** fatal level message
*/
void log_fatal(const char *file_name, int line, const char *message) {
  assert(message != (char *) NULL);
  syslog(LOG_EMERG, "%s %d %s", file_name, line, message);
  fprintf(stderr, "FATAL:%s\n", message);
  exit(EXIT_FAILURE);
}

/*
** syslog initialization
*/
void log_setup(const char *identifier) {
  assert(identifier != (char *) NULL);
  openlog(identifier, LOG_CONS|LOG_PID, LOG_LOCAL5);
}

/*
** orderly shutdown
*/
void log_shutdown(void) {
  closelog();
}
