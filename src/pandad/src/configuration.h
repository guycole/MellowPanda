/*
** Title:configuration.h
**
** Description:
**   Global configuration structure.
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

#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <time.h>

#include "utility.h"

/*
** configuration defaults
*/
#define DEFAULT_COMMAND_PORT 5166  //tcp
#define DEFAULT_UPDATE_PORT 5166   //udp
#define DEFAULT_SERVER_NAME "localhost"

#define DEFAULT_CONFIGURATION_DIRECTORY "/usr/local/etc"
#define DEFAULT_CONFIGURATION_FILE "pandad.conf"

/*
** configuration file keywords
*/
#define KEY_COMMAND_PORT "command_port"
#define KEY_COMMAND_SERVER "command_server"

#define KEY_DEVICE "device"
#define KEY_DEVICE_ACTIVE "active"
#define KEY_DEVICE_DELIM "delim"
#define KEY_DEVICE_NAME "name"
#define KEY_DEVICE_PARAMZ "paramz"
#define KEY_DEVICE_PORT "port"
#define KEY_DEVICE_SERIAL "serial"
#define KEY_DEVICE_SPEED "speed"
#define KEY_DEVICE_TYPE "type"

#define MAX_KEY_DEVICE_COMPONENT 3

/*
** global configuration structure
*/
typedef struct {
  time_t boot_time;          //daemon start time
  time_t reset_time;         //daemon reset time

  BOOLEAN diagnostic_mode;   //true, write diagnostics and exit
  BOOLEAN foreground_mode;   //true, run in foreground (do not fork)
  BOOLEAN write_version;     //true, write version string and exit

  int pid_lock_fd;           //PID lock file

  int command_fd;            //command file descriptor;
  int command_port;          //command port

  int next_update_port;      //UDP update port assignment

  char *server_name;         //server name

  char *configuration_file_name;

  char *experiment;          //experimental command string
} CONFIGURATION, *CONFIGURATION_PTR;

/*
** establish configuration defaults
*/
extern void configuration_defaults(void);

/*
** read and parse the configuration file
** file contents are used to populate the global configuration structure
*/
extern void configuration_read(void);

/*
** return current configuration
*/
extern void configuration_report(char *buffer);

#endif /* __CONFIGURATION_H__ */
