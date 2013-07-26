/*
** Title:socket.h
**
** Description:
**   Support for the command socket (commands between client and server)
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

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "device.h"

#define MAX_SOCKET_BACKLOG 10

/*
** write a command to the server and obtain a response
*/
extern void socket_command(char *request, char *response);

/*
** given a file descriptor, return a message
*/
extern void socket_reader(int fd, char *message);

/*
** establish the server command socket and return associated file descriptor
*/
extern int socket_server(void);

/*
** establish a broadcast update UDP socket
*/
extern int socket_update_create(SERIAL_DEVICE_PTR device);

/*
** write broadcast message
*/
extern void socket_update_write(SERIAL_DEVICE_PTR device);

#endif /* __SOCKET_H__ */
