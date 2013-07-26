/*
** Title:bearcat.c
**
** Description:
**   Support for Uniden Bearcat radios
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

#include "bearcat.h"
#include "log.h"
#include "rs232.h"

/*
** read all pending responses from serial port
*/
void bearcat_read(SERIAL_DEVICE_PTR device) {
  char log_message[MAX_LOG_MESSAGE];

  assert(device != (SERIAL_DEVICE_PTR) NULL);

  read_port(device->serial_descriptor, device->rx_buffer);

  sprintf(log_message, "unsolicited read:%d:%s:%s", device->serial_descriptor, device->name, device->rx_buffer);
  log_info(__FILE__, __LINE__, log_message);

  if (strlen(device->rx_buffer)) {
    socket_update_write(device);
  }
}

/*
** service a request/response cycle
*/
void bearcat_serial_read_response(SERIAL_DEVICE_PTR device) {
  int fd;
  char delimiter;

  assert(device != (SERIAL_DEVICE_PTR) NULL);

  fd = device->serial_descriptor;
  delimiter = utility_delimiter(device->serial_delimiter);

  write_port(fd, delimiter, device->tx_buffer);
  read_port(fd, device->rx_buffer);

  if (strlen(device->rx_buffer)) {
    socket_update_write(device);
  }
}

/*
** service a remote command
*/
void bearcat_service(SERIAL_DEVICE_PTR device, SERIAL_COMMAND_TYPE command, char *argument, char *response) {
  char log_message[MAX_LOG_MESSAGE];

  assert(device != (SERIAL_DEVICE_PTR) NULL);

  switch (command) {
  case PING_COMMAND:
    strcpy(device->tx_buffer, "SI");
    bearcat_serial_read_response(device);
    strcpy(response, device->rx_buffer);
    break;
  case RAW_COMMAND:
    strcpy(device->tx_buffer, argument);
    bearcat_serial_read_response(device);
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
void bearcat_init(SERIAL_DEVICE_PTR device) {
  char res_buffer[MAX_RESPONSE_BUFFER];

  assert(device != (SERIAL_DEVICE_PTR) NULL);

  bearcat_service(device, PING_COMMAND, NULL, res_buffer);
}

/*
** graceful exit
*/
void bearcat_shutdown(SERIAL_DEVICE_PTR device) {
  assert(device != (SERIAL_DEVICE_PTR) NULL);
}

/*
** commands to invoke when idle
*/
void bearcat_timeout(SERIAL_DEVICE_PTR device) {
  assert(device != (SERIAL_DEVICE_PTR) NULL);
}
