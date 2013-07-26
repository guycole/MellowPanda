/*
** Title:configuration.c
**
** Description:
**   Global configuration support such as reading configuration file.
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

#define _GNU_SOURCE  //need for Ubuntu 8

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/param.h>

#include "configuration.h"
#include "device.h"
#include "log.h"

CONFIGURATION gConfiguration;

/*
** establish configuration defaults
*/
void configuration_defaults(void) {
  char buffer[PATH_MAX];

  gConfiguration.reset_time = time(NULL);

  gConfiguration.pid_lock_fd = -1;

  gConfiguration.command_fd = -1;
  gConfiguration.command_port = DEFAULT_COMMAND_PORT;

  gConfiguration.next_update_port = DEFAULT_UPDATE_PORT;

  if (gConfiguration.server_name != NULL) {
    free(gConfiguration.server_name);
  }
  gConfiguration.server_name = strdup(DEFAULT_SERVER_NAME);

  if (gConfiguration.configuration_file_name != NULL) {
    free(gConfiguration.configuration_file_name);
  }

  //
  // CONFIGURATION_DIRECTORY set from Makefile
  //
  sprintf(buffer, "%s/%s", CONFIGURATION_DIRECTORY, DEFAULT_CONFIGURATION_FILE);
  gConfiguration.configuration_file_name = strdup(buffer);

  gConfiguration.experiment = NULL;

  gConfiguration.write_version = FALSE;
  gConfiguration.diagnostic_mode = FALSE;
  gConfiguration.foreground_mode = FALSE;
}

/*
** set a serial device configuration item
** key is a tuple in the form "device.ndx.key"
** parse and validate in this method
*/
void configuration_device_set(const char *key, const char *value) {
  int ii;
  char *cptr1, *cptr2;
  char *device_key[MAX_KEY_DEVICE_COMPONENT];
  char log_message[MAX_LOG_MESSAGE];
  SERIAL_DEVICE_PTR device_ptr;

  assert(key != (char *) NULL);
  assert(value != (char *) NULL);

  bzero(device_key, sizeof(device_key));

  //extract tokens
  cptr1 = strtok((char *) key, ".");
  for (ii = 0; ((cptr1 != NULL) && (ii < MAX_KEY_DEVICE_COMPONENT)); ii++) {
    device_key[ii] = cptr1; 
    cptr1 = strtok(NULL, ".");      
  }

  //ensure all tokens are populated
  for (ii = 0; ii < MAX_KEY_DEVICE_COMPONENT; ii++) {
    if (device_key[ii] == NULL) {
      sprintf(log_message, "bad device key:%s", key);
      log_error(__FILE__, __LINE__, log_message);
      return;
    }
  }

  //validate first token
  if (strncasecmp(device_key[0], KEY_DEVICE, strlen(KEY_DEVICE))) {
      sprintf(log_message, "bad device key:%s", key);
      log_error(__FILE__, __LINE__, log_message);
      return;
  }

  //extract middle token
  int ndx = atoi(device_key[1]);

  //validate last token
  cptr2 = device_key[2];
  device_ptr = get_device_by_ndx(ndx);
  if (device_ptr == NULL) {
    device_ptr = get_fresh_device();
    device_ptr->ndx = ndx;
  }

  if (!strncasecmp(cptr2, KEY_DEVICE_NAME, strlen(KEY_DEVICE_NAME))) {
    free(device_ptr->name);
    device_ptr->name = strdup(value);
  } else if (!strncasecmp(cptr2, KEY_DEVICE_SERIAL, strlen(KEY_DEVICE_SERIAL))) {
    free(device_ptr->serial_port);
    device_ptr->serial_port = strdup(value);
  } else if (!strncasecmp(cptr2, KEY_DEVICE_SPEED, strlen(KEY_DEVICE_SPEED))) {
    free(device_ptr->serial_speed);
    device_ptr->serial_speed = strdup(value);
  } else if (!strncasecmp(cptr2, KEY_DEVICE_DELIM, strlen(KEY_DEVICE_DELIM))) {
    device_ptr->serial_delimiter = utility_map_delimiter(value);
  } else if (!strncasecmp(cptr2, KEY_DEVICE_PARAMZ, strlen(KEY_DEVICE_PARAMZ))) {
    free(device_ptr->serial_paramz);
    device_ptr->serial_paramz = strdup(value);
  } else if (!strncasecmp(cptr2, KEY_DEVICE_TYPE, strlen(KEY_DEVICE_TYPE))) {
    device_ptr->type = utility_map_device_type(value);
  } else if (!strncasecmp(cptr2, KEY_DEVICE_PORT, strlen(KEY_DEVICE_PORT))) {
    device_ptr->update_port = atoi(value);
  } else if (!strncasecmp(cptr2, KEY_DEVICE_ACTIVE, strlen(KEY_DEVICE_ACTIVE))) {
    device_ptr->active = utility_map_boolean(value);
  } else {
      sprintf(log_message, "bad device key:%s", device_key[2]);
      log_error(__FILE__, __LINE__, log_message);
      return;
  }
}

/*
** set a simple configuration item
*/
void configuration_simple_set(const char *key, const char *value) {
  assert(key != (char *) NULL);
  assert(value != (char *) NULL);

  char *cptr1;

  char log_message[MAX_LOG_MESSAGE];

  sprintf(log_message, "key:%s:value:%s", key, value);
  //  log_debug(__FILE__, __LINE__, log_message);

  if (!strncasecmp(key, KEY_COMMAND_PORT, strlen(KEY_COMMAND_PORT))) {
    gConfiguration.command_port = atoi(value);
  }

  if (!strncasecmp(key, KEY_COMMAND_SERVER, strlen(KEY_COMMAND_SERVER))) {
    gConfiguration.server_name = strdup(value);
  }

  //
  // device definition
  //
  cptr1 = (char *) strcasestr(key, KEY_DEVICE);
  if (cptr1 != (char *) NULL) {
    configuration_device_set(key, value);
  }
}

/*
** read and parse the configuration file
** file contents are used to populate the global configuration structure
*/
void configuration_read(void) {
  FILE *file;

  char buffer[1024];
  char *cptr1, *key, *value;

  char log_message[MAX_LOG_MESSAGE];

  if ((file = fopen(gConfiguration.configuration_file_name, "r")) == NULL) {
    sprintf(log_message, "unable to read configuration file %s", gConfiguration.configuration_file_name);
    log_fatal(__FILE__, __LINE__, log_message);
    return;
  }

  free_device_root();

  while((!feof(file)) && (fgets(buffer, sizeof(buffer), file))) {
    // remove comments
    if ((cptr1 = strchr(buffer, '#')) != NULL) {
      *cptr1 = '\0';  
    }

    // ignore empty lines
    if (strlen(buffer) < 2) {
      continue;
    }

    // parse key value pairs
    key = strtok(buffer, " =\t\n");
    if (key == NULL) {
      log_error(__FILE__, __LINE__, "null key in configuration file\n");
    } else {
      value = strtok(NULL, " =\t\n");
      if (value == NULL) {
	sprintf(log_message, "null value for key %s\n", key);
	log_error(__FILE__, __LINE__, log_message);
      } else {
	configuration_simple_set(key, value);
      }
    }
  }

  fclose(file);
}

/*
** return the current configuration
*/
void configuration_report(char *buffer) {
  time_t time_now;
  SERIAL_DEVICE_PTR device;

  char temp1[128];
  char temp2[128];
  char temp3[128];

  time_now = time(NULL);

  sprintf(temp1, "status on %s", ctime(&time_now));
  strcat(buffer, temp1);

  pandad_version_string(temp1);
  sprintf(temp2, "version:%s\n", temp1);
  strcat(buffer, temp2);

  sprintf(temp1, "booted on %s", ctime(&gConfiguration.boot_time));
  strcat(buffer, temp1);

  sprintf(temp1, "reset on %s", ctime(&gConfiguration.reset_time));
  strcat(buffer, temp1);

  sprintf(temp2, "host:%s command port:%d command fd:%d update port:%d\n", gConfiguration.server_name, gConfiguration.command_port, gConfiguration.command_fd, gConfiguration.next_update_port);
  strcat(buffer, temp2);

  if (gConfiguration.experiment == NULL) {
    sprintf(temp2, "experimental string:NULL\n");
  } else {
    sprintf(temp2, "experimental string:%s\n", gConfiguration.experiment);
  }
  strcat(buffer, temp2);

  if (gConfiguration.diagnostic_mode) {
    strcat(buffer, "diagnostic mode:true\n");
  } else {
    strcat(buffer, "diagnostic mode:false\n");
  }

  if (gConfiguration.foreground_mode) {
    strcat(buffer, "foreground mode:true\n");
  } else {
    strcat(buffer, "foreground mode:false\n");
  }

  if (gConfiguration.write_version) {
    strcat(buffer, "write version:true\n");
  } else {
    strcat(buffer, "write version:false\n");
  }

  if (gConfiguration.configuration_file_name == NULL) {
    sprintf(temp2, "configuration filename:NULL\n");
  } else {
    sprintf(temp2, "configuration filename:%s\n", gConfiguration.configuration_file_name);
  }
  strcat(buffer, temp2);

  device = get_device_root();
  while (device != NULL) {
    utility_map_device_type_to_string(device->type, temp1);
    utility_map_delimiter_to_string(device->serial_delimiter, temp3);

    sprintf(temp2, "device:%d name:%s type:%s port:%s fd:%d speed:%s params:%s delim:%s udp:%d\n", device->ndx, device->name, temp1, device->serial_port, device->serial_descriptor, device->serial_speed, device->serial_paramz, temp3, device->update_port);
    strcat(buffer, temp2);

    if (device->active) {
      strcat(buffer, "this device is enabled\n");
    } else {
      strcat(buffer, "this device is disabled\n");
    }

    device = device->next;
  }
}
