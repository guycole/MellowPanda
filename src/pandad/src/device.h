/*
** Title:device.h
**
** Description:
**   serial device facade
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

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <sys/time.h>

#include "utility.h"

#define DEFAULT_DEVICE_NAME "Unknown"
#define DEFAULT_SERIAL_SPEED "B9600"
#define DEFAULT_SERIAL_PARAMZ "8N1"
#define DEFAULT_SERIAL_DELIMITER LF_DELIMITER
#define DEFAULT_SERIAL_PORT "Unknown"

#define RESPONSE_BAD_DEVICE "device_error"
#define RESPONSE_UNSUPPORTED "device_unsupported"
#define RESPONSE_OK_DEVICE "device_ok"

/*
**
*/
typedef struct radio {
  int ndx;                  //unique device index from configuration file
  BOOLEAN active;           //true, this radio is working
  SERIAL_DEVICE_TYPE type;  //enumerated device type
  char *name;               //unique device name

  char *serial_port;        //serial port
  char *serial_speed;       //serial port speed, i.e. B9600, B38400, etc
  char *serial_paramz;      //serial port parameters, i.e. 8N1, 7E2, etc
  int serial_descriptor;    //serial port file descriptor
  SERIAL_COMMAND_DELIMITER serial_delimiter;

  int update_port;          //update port assignment
  int update_descriptor;    //update file descriptor

  char rx_buffer[MAX_RESPONSE_BUFFER];
  char tx_buffer[MAX_REQUEST_BUFFER];

  struct radio *next;       //next radio or NULL
} SERIAL_DEVICE, *SERIAL_DEVICE_PTR;

/*
** return root pointer to device list
*/
extern SERIAL_DEVICE_PTR get_device_root(void);

/*
** gracefully release all devices 
*/
extern void free_device_root(void);

/*
** return a device or null if not found
*/
extern SERIAL_DEVICE_PTR get_device_by_ndx(int ndx);

/*
** return a device or null if not found
*/
extern SERIAL_DEVICE_PTR get_device_by_name(char *name);

/*
** return a freshly alloc/initialized device
*/
extern SERIAL_DEVICE_PTR get_fresh_device(void);

/*
** open all serial devices
*/
extern void device_open_all(void);

/*
** close all serial devices
*/
extern void device_close_all(void);

/*
** time out hook
*/
extern void device_timeout(void);

/*
** read data from serial port
*/
extern void device_read(SERIAL_DEVICE_PTR device);

/*
** service a remote command
*/
extern void device_service(SERIAL_DEVICE_PTR device, SERIAL_COMMAND_TYPE command, char *argument, char *response);

#endif /* __DEVICE_H__ */
