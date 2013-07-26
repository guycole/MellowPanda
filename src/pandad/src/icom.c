/*
** Title:icom.c
**
** Description:
**   ICOM radio support (PCR-1000, PCR-1500)
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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "icom.h"
#include "log.h"
#include "rs232.h"
#include "socket.h"

/*
** read all pending responses from serial port
*/
void icom_read(SERIAL_DEVICE_PTR device) {
  int attempts, limit;

  assert(device != (SERIAL_DEVICE_PTR) NULL);

  /*
  ** it is normal for icom to have extra cr/lf in the read buffer
  ** must consume extra cr/lf to obtain message
  */
  for (attempts = 0, limit = 0; ((attempts < 5) && (limit < 1)); attempts++) {
    read_port(device->serial_descriptor, device->rx_buffer);
    limit = strlen(device->rx_buffer);
  }

  if (strlen(device->rx_buffer)) {
    socket_update_write(device);
  }
}

/*
** service a request/response cycle
*/
void icom_serial_read_response(SERIAL_DEVICE_PTR device) {
  int fd;
  char delimiter;

  assert(device != (SERIAL_DEVICE_PTR) NULL);

  fd = device->serial_descriptor;
  delimiter = utility_delimiter(device->serial_delimiter);

  write_port(fd, delimiter, device->tx_buffer);

  icom_read(device);
}

/*
** service a remote command
*/
void icom_service(SERIAL_DEVICE_PTR device, SERIAL_COMMAND_TYPE command, char *argument, char *response) {
  char log_message[MAX_LOG_MESSAGE];

  assert(device != (SERIAL_DEVICE_PTR) NULL);

  switch (command) {
  case PING_COMMAND:
    strcpy(device->tx_buffer, "H1?");
    icom_serial_read_response(device);
    strcpy(response, device->rx_buffer);
    break;
  case RAW_COMMAND:
    strcpy(device->tx_buffer, argument);
    icom_serial_read_response(device);
    strcpy(response, device->rx_buffer);
    break;
  default:
    sprintf(log_message, "unknown command:%d:%s", command, device->name);
    log_error(__FILE__, __LINE__, log_message);
  }
}

/*
** initialize this radio
*/
void icom_init(SERIAL_DEVICE_PTR device) {
  char res_buffer[MAX_RESPONSE_BUFFER];
  assert(device != (SERIAL_DEVICE_PTR) NULL);
  icom_service(device, PING_COMMAND, NULL, res_buffer);
}

/*
** graceful shutdown
*/
void icom_shutdown(SERIAL_DEVICE_PTR device) {
  assert(device != (SERIAL_DEVICE_PTR) NULL);
}

/*
** commands to invoke when idle
*/
void icom_timeout(SERIAL_DEVICE_PTR device) {
  //char res_buffer[MAX_RESPONSE_BUFFER];
  assert(device != (SERIAL_DEVICE_PTR) NULL);
  //  icom_service(device, PING_COMMAND, NULL, res_buffer);
}
