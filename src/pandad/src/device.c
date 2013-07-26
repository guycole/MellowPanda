/*
** Title:device.c
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
**   $Log$
*/
static char rcsid[] = "$Id$";

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <syslog.h>
#include <time.h>
#include <unistd.h>

#include "bearcat.h"
#include "configuration.h"
#include "icom.h"
#include "log.h"
#include "rs232.h"
#include "socket.h"
#include "utility.h"

extern CONFIGURATION gConfiguration;

static SERIAL_DEVICE_PTR gDeviceRootPtr = NULL;

/*
** return root pointer to device list
*/
SERIAL_DEVICE_PTR get_device_root(void) {
  return(gDeviceRootPtr);
}

/*
** gracefully release all devices 
*/
void free_device_root(void) {
  SERIAL_DEVICE_PTR current = gDeviceRootPtr;

  while (current != NULL) {
    free(current->name);
    free(current->serial_port);
    free(current->serial_speed);
    free(current->serial_paramz);
    free(current);

    current = current->next;
  }

  gDeviceRootPtr = NULL;
}

/*
** return a radio or NULL if not found
*/
SERIAL_DEVICE_PTR get_device_by_ndx(int ndx) {
  SERIAL_DEVICE_PTR current = gDeviceRootPtr;

  while (current != NULL) {
    if (current->ndx == ndx) {
      return(current);
    }

    current = current->next;
  }

  return(NULL);
}

/*
** return a device or NULL if not found
*/
SERIAL_DEVICE_PTR get_device_by_name(char *name) {
  SERIAL_DEVICE_PTR current = gDeviceRootPtr;

  assert(name != (char *) NULL);

  while (current != NULL) {
    if (!strcmp(current->name, name)) {
      return(current);
    }

    current = current->next;
  }

  return(NULL);
}

/*
** return a freshly alloc/initialized device
*/
SERIAL_DEVICE_PTR get_fresh_device(void) {
  SERIAL_DEVICE_PTR serial_ptr;

  serial_ptr = (SERIAL_DEVICE_PTR) malloc(sizeof(SERIAL_DEVICE));

  serial_ptr->ndx = 0;
  serial_ptr->active = FALSE;

  serial_ptr->type = UNKNOWN_DEVICE;

  serial_ptr->name = strdup(DEFAULT_DEVICE_NAME);

  serial_ptr->serial_descriptor = -1;
  serial_ptr->serial_port = strdup(DEFAULT_SERIAL_PORT);
  serial_ptr->serial_speed = strdup(DEFAULT_SERIAL_SPEED);
  serial_ptr->serial_paramz = strdup(DEFAULT_SERIAL_PARAMZ);
  serial_ptr->serial_delimiter = DEFAULT_SERIAL_DELIMITER;

  serial_ptr->update_descriptor = -1;
  serial_ptr->update_port = gConfiguration.next_update_port++;

  serial_ptr->next = gDeviceRootPtr;
  gDeviceRootPtr = serial_ptr;
  
  return(serial_ptr);
}

/*
** close all device ports
*/
void device_close_all(void) {
  SERIAL_DEVICE_PTR device;
  char log_message[MAX_LOG_MESSAGE];

  device = gDeviceRootPtr; 

  while (device != NULL) {
    if (device->active) {
      switch(device->type) {
      case PCR1000:
	icom_shutdown(device);
	break;
      case PCR1500:
	icom_shutdown(device);
	break;
      case BC895:
	bearcat_shutdown(device);
	break;
      case BC245:
	bearcat_shutdown(device);
	break;
      case BC780:
	bearcat_shutdown(device);
	break;
      case PRO2052:
	bearcat_shutdown(device);
	break;
      default:
	sprintf(log_message, "unknown device type:%d:%s", device->ndx, device->name);
	log_info(__FILE__, __LINE__, log_message);
      }

      close(device->serial_descriptor);
      close(device->update_descriptor);

      device->active = FALSE;
      device->serial_descriptor = -1;
      device->update_descriptor = -1;
    }

    device = device->next;
  }
}

/*
** send initialization commands to device
*/
void device_initialize(SERIAL_DEVICE_PTR device) {
  char log_message[MAX_LOG_MESSAGE];

  assert(device != (SERIAL_DEVICE_PTR) NULL);

  switch(device->type) {
  case PCR1000:
    icom_init(device);
    break;
  case PCR1500:
    icom_init(device);
    break;
  case BC895:
    bearcat_init(device);
    break;
  case BC245:
    bearcat_init(device);
    break;
  case BC780:
    bearcat_init(device);
    break;
  case PRO2052:
    bearcat_init(device);
    break;
  default:
    sprintf(log_message, "unknown device type:%d:%s", device->ndx, device->name);
    log_error(__FILE__, __LINE__, log_message);
  }
}

/*
** open all device ports
*/
void device_open_all(void) {
  int fd;
  SERIAL_DEVICE_PTR device;
  char log_message[MAX_LOG_MESSAGE];

  device = gDeviceRootPtr;

  while (device != NULL) {
    if (device->active == FALSE) {
      sprintf(log_message, "skip open:%s:%s", device->serial_port, device->name);
      log_debug(__FILE__, __LINE__, log_message);
    } else {
      fd = open_port(device->serial_port, device->serial_speed, device->serial_paramz);
      sprintf(log_message, "open port:%d:%s:%s", fd, device->serial_port, device->name);
      log_debug(__FILE__, __LINE__, log_message);

      if (fd < 0) {
	sprintf(log_message, "open port failure:%s:%s:", device->serial_port, device->name);
	log_error(__FILE__, __LINE__, log_message);
      } else {
	device->active = TRUE;
	device->serial_descriptor = fd;

	device->update_descriptor = socket_update_create(device);

	device_initialize(device);
      }
    }

    device = device->next;
  }
}

/*
** time out hook
*/
void device_timeout(void) {
  SERIAL_DEVICE_PTR device;
  char log_message[MAX_LOG_MESSAGE];

  //  printf("device timeout\n");

  device = gDeviceRootPtr;

  while (device != NULL) {
    if (device->active) {
      switch(device->type) {
      case PCR1000:
	icom_timeout(device);
	break;
      case PCR1500:
	icom_timeout(device);
	break;
      case BC895:
	bearcat_timeout(device);
	break;
      case BC245:
	bearcat_timeout(device);
	break;
      case BC780:
	bearcat_timeout(device);
	break;
      case PRO2052:
	bearcat_timeout(device);
	break;
      default:
	sprintf(log_message, "unknown device type:%d:%s", device->ndx, device->name);
	log_error(__FILE__, __LINE__, log_message);
      }
    }

    device = device->next;
  }
}

/*
** read fresh data from serial
*/
void device_read(SERIAL_DEVICE_PTR device) {
  char log_message[MAX_LOG_MESSAGE];

  assert(device != (SERIAL_DEVICE_PTR) NULL);

  switch(device->type) {
  case PCR1000:
    icom_read(device);
    break;
  case PCR1500:
    icom_read(device);
    break;
  case BC895:
    bearcat_read(device);
    break;
  case BC245:
    bearcat_read(device);
    break;
  case BC780:
    bearcat_read(device);
    break;
  case PRO2052:
    bearcat_read(device);
    break;
  default:
    sprintf(log_message, "unknown device type:%d:%s", device->ndx, device->name);
    log_error(__FILE__, __LINE__, log_message);
  }
}

/*
**
*/
void device_service(SERIAL_DEVICE_PTR device, SERIAL_COMMAND_TYPE command, char *argument, char *response) {
  char log_message[MAX_LOG_MESSAGE];

  assert(device != (SERIAL_DEVICE_PTR) NULL);

  sprintf(log_message, "device service:%s:%d:", device->name, device->active);
  log_debug(__FILE__, __LINE__, log_message);

  if (!device->active) {
    log_debug(__FILE__, __LINE__, "ret w/inactive device");
    strcpy(response, RESPONSE_BAD_DEVICE);
    return;
  }

  switch(device->type) {
  case PCR1000:
    icom_service(device, command, argument, response);
    break;
  case PCR1500:
    icom_service(device, command, argument, response);
    break;
  case BC895:
    bearcat_service(device, command, argument, response);
    break;
  case BC245:
    bearcat_service(device, command, argument, response);
    break;
  case BC780:
    bearcat_service(device, command, argument, response);
    break;
  case PRO2052:
    bearcat_service(device, command, argument, response);
    break;
  default:
    sprintf(log_message, "unknown device type:%s %d", device->name, device->ndx);
    log_error(__FILE__, __LINE__, log_message);
    strcpy(response, RESPONSE_BAD_DEVICE);
  }
}
