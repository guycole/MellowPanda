/*
** Title:command.c
**
** Description:
**   Support for command parsing and dispatch.
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

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "command.h"
#include "configuration.h"
#include "device.h"
#include "log.h"

extern CONFIGURATION gConfiguration;

/*
** return auto update port assignment
*/
void auto_update_command(char *name, char *response) {
  int ii, limit;
  SERIAL_DEVICE_PTR device;
  char log_message[MAX_LOG_MESSAGE];

  assert(name != (char *) NULL);
  assert(response != (char *) NULL);

  //remove control characters (if any)
  limit = strlen(name);
  for (ii = 0; ii < limit; ii++) {
    if (iscntrl(name[ii])) {
      name[ii] = '\0';
    }
  }

  device = get_device_by_name(name);
  if (device == NULL) {
    sprintf(log_message, "%s:%s", RESPONSE_BAD_DEVICE, name);
    log_error(__FILE__, __LINE__, log_message);
    strcpy(response, RESPONSE_BAD_DEVICE);
    return;
  }

  sprintf(response, "%s/%s/%d/%d", PROTO_KEY_AUTO_UPDATE, device->name, device->update_descriptor, device->update_port);
}

/*
** return server version string
*/
void version_command(char *buffer) {
  char temp1[MAX_RESPONSE_BUFFER];

  pandad_version_string(temp1);
  strcat(temp1, "\n");
  strcpy(buffer, temp1);
}

/*
** ping a serial device
*/
void ping_command(char *name, char *response) {
  int ii, limit;
  SERIAL_DEVICE_PTR device;
  char log_message[MAX_LOG_MESSAGE];

  assert(name != (char *) NULL);
  assert(response != (char *) NULL);

  //remove control characters (if any)
  limit = strlen(name);
  for (ii = 0; ii < limit; ii++) {
    if (iscntrl(name[ii])) {
      name[ii] = '\0';
    }
  }

  device = get_device_by_name(name);
  if (device == NULL) {
    sprintf(log_message, "%s:%s", RESPONSE_BAD_DEVICE, name);
    log_error(__FILE__, __LINE__, log_message);
    strcpy(response, RESPONSE_BAD_DEVICE);
    return;
  }

  device_service(device, PING_COMMAND, NULL, response);
}

/*
** validate command and dispatch for processing
*/
void device_command(char *name, char *command, char *argument, char *response) {
  int ii, limit;
  SERIAL_DEVICE_PTR device;
  char log_message[MAX_LOG_MESSAGE];

  assert(name != (char *) NULL);
  assert(command != (char *) NULL);
  assert(argument != (char *) NULL);
  assert(response != (char *) NULL);

  //remove control characters (if any)
  limit = strlen(name);
  for (ii = 0; ii < limit; ii++) {
    if (iscntrl(name[ii])) {
      name[ii] = '\0';
    }
  }

  device = get_device_by_name(name);
  if (device == NULL) {
    sprintf(log_message, "%s:%s", RESPONSE_BAD_DEVICE, name);
    log_error(__FILE__, __LINE__, log_message);
    strcpy(response, RESPONSE_BAD_DEVICE);
    return;
  }

  //remove control characters (if any)
  limit = strlen(command);
  for (ii = 0; ii < limit; ii++) {
    if (iscntrl(command[ii])) {
      command[ii] = '\0';
    }
  }

  SERIAL_COMMAND_TYPE sct = utility_map_serial_command(command);
  if (sct == UNKNOWN_COMMAND) {
    sprintf(log_message, "%s:%s", RESPONSE_BAD_COMMAND, command);
    log_error(__FILE__, __LINE__, log_message);
    strcpy(response, RESPONSE_BAD_COMMAND);
    return;
  }

  //remove control characters (if any)
  limit = strlen(argument);
  for (ii = 0; ii < limit; ii++) {
    if (iscntrl(argument[ii])) {
      argument[ii] = '\0';
    }
  }

  device_service(device, sct, argument, response);
}

/*
** process a remote command
*/
void command(char *request, char *response) {
  int max_argz;
  char *cptr1;
  char *argz[MAX_ARGZ];
  char *argument, *command, *name;
  char log_message[MAX_LOG_MESSAGE];

  assert(request != (char *) NULL);
  assert(response != (char *) NULL);

  log_debug(__FILE__, __LINE__, request);

  /*
  ** short length test
  */
  if (strlen(request) < strlen(PANDA_PROTO)) {
    log_error(__FILE__, __LINE__, RESPONSE_BAD_FORMAT);
    strcpy(response, RESPONSE_BAD_FORMAT);
    return;
  }

  /*
  ** server configuration request
  */
  if (!strncmp(request, REQUEST_STATUS, strlen(REQUEST_STATUS))) {
    configuration_report(response);
    return;
  }

  /*
  ** server version request
  */
  if (!strncmp(request, REQUEST_VERSION, strlen(REQUEST_VERSION))) {
    version_command(response);
    return;
  }

  /*
  ** ...parse request...
  */

  max_argz = 0;
  cptr1 = strtok((char *) request, "/");
  while (cptr1 != NULL) {
    //printf("---%d:%s\n", max_argz, cptr1);
    argz[max_argz++] = cptr1;
    cptr1 = strtok(NULL, "/");
  }

  if (strncmp(argz[0], PANDA_PROTO, strlen(PANDA_PROTO))) {
    log_error(__FILE__, __LINE__, RESPONSE_BAD_FORMAT);
    strcpy(response, RESPONSE_BAD_FORMAT);
    return;
  }

  //
  // panda://auto_update/radio1bc
  //
  if (!strncmp(argz[1], PROTO_KEY_AUTO_UPDATE, strlen(PROTO_KEY_AUTO_UPDATE))) {
    if (max_argz == 3) {
      auto_update_command(argz[2], response);
      return;
    }
  }

  //
  // panda://ping/radio1bc
  //
  if (!strncmp(argz[1], PROTO_KEY_PING, strlen(PROTO_KEY_PING))) {
    if (max_argz == 3) {
      ping_command(argz[2], response);
      return;
    }
  }

  if (max_argz < 4) {
    sprintf(log_message, "short argument count:%d", max_argz);
    log_error(__FILE__, __LINE__, log_message);
    strcpy(response, RESPONSE_BAD_FORMAT);
    return;
  } 

  //
  // panda://device/raw/radio1bc/SI
  // panda://device/raw/radio1bc/PM002 F01322000
  //
  if (!strncmp(argz[1], PROTO_KEY_DEVICE, strlen(PROTO_KEY_DEVICE))) {
    command = argz[2];
    name = argz[3];
    argument = argz[4];

    if (argument == NULL) {
      strcpy(response, RESPONSE_BAD_FORMAT);
      return;
    }

    device_command(name, command, argument, response);

    return;
  }

  //
  // default
  //
  log_error(__FILE__, __LINE__, RESPONSE_BAD_FORMAT);
  strcpy(response, RESPONSE_BAD_FORMAT);
}
