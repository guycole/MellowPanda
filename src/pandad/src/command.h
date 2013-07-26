/*
** Title:command.h
**
** Description:
**   Support for command parsing and dispatch
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

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "utility.h"

#define MAX_ARGZ 10

#define PANDA_PROTO "panda:"
#define PROTO_KEY_AUTO_UPDATE "auto_update"
#define PROTO_KEY_DEVICE "device"
#define PROTO_KEY_PING "ping"
#define PROTO_KEY_RAW "raw"

#define REQUEST_STATUS "panda://status"
#define REQUEST_UPDATE_SOCKET "panda://autoupdate"
#define REQUEST_VERSION "panda://version"

#define RESPONSE_BAD_COMMAND "command_error"
#define RESPONSE_BAD_FORMAT "format_error"
#define RESPONSE_OK "ok"

/*
** process a remote command
*/
extern void command(char *request, char *response);

#endif /* __COMMAND_H__ */
