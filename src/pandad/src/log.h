/*
** Title:log.h
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
**   $Id$
**
**   $Log$
*/

#ifndef __LOG_H__
#define __LOG_H__

#define MAX_LOG_MESSAGE 1024

/*
** debug level message
*/
extern void log_debug(const char *file_name, int line, const char *message);

/*
** informational level message
*/
extern void log_info(const char *file_name, int line, const char *message);

/*
** error level message
*/
extern void log_error(const char *file_name, int line, const char *message);

/*
** fatal level message
*/
extern void log_fatal(const char *file_name, int line, const char *message);

/*
** syslog initialization
*/
extern void log_setup(const char *identifier);

/*
** orderly shutdown
*/
extern void log_shutdown(void);

#endif /* __LOG_H__ */
