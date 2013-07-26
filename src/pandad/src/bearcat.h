/*
** Title:bearcat.h
**
** Description:
**   Support for Uniden Bearcat radios.
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

#ifndef __BEARCAT_H__
#define __BEARCAT_H__

#include "device.h"

/*
** initialize this radio
*/
extern void bearcat_init(SERIAL_DEVICE_PTR device);

/*
** read all pending responses from serial port
*/
extern void bearcat_read(SERIAL_DEVICE_PTR device);

/*
** graceful exit
*/
extern void bearcat_shutdown(SERIAL_DEVICE_PTR device);

/*
** commands to invoke when idle
*/
extern void bearcat_timeout(SERIAL_DEVICE_PTR device);

/*
** service a remote command
*/
extern void bearcat_service(SERIAL_DEVICE_PTR device, SERIAL_COMMAND_TYPE command, char *argument, char *response);

#endif /* __BEARCAT_H__ */
