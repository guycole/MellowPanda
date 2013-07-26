/*
** Title:eclectic.c
**
** Description:
**   poll file descriptors and react to arriving data.
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

#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "command.h"
#include "device.h"
#include "log.h"
#include "socket.h"

/*
** maintenance, runs between select(2) calls
*/
void maintainer(void) {
  char log_message[MAX_LOG_MESSAGE];

  sprintf(log_message, "xxx maintainer xxx");
  //  log_error(__FILE__, __LINE__, log_message);

  device_timeout();
}

/*
** Extract the remote command and dispatch for processing
** Write the response to the remote client.
*/
void responder(int fd) {
  char req_buffer[MAX_REQUEST_BUFFER];
  char res_buffer[MAX_RESPONSE_BUFFER];

  bzero(req_buffer, sizeof(req_buffer));
  bzero(res_buffer, sizeof(res_buffer));

  socket_reader(fd, req_buffer);

  command(req_buffer, res_buffer);

  if (write(fd, res_buffer, strlen(res_buffer)) < 0) {
    log_error(__FILE__, __LINE__, "write failure");
  }
}

/*
** select(2) loop
**
** one command socket (from client)
** serial port file descriptors from each radio
*/
void eclectic(void) {
  int ret_val;
  int connect_fd;
  int max_read_fd;
  fd_set xcept_fds, read_fds, write_fds;

  SERIAL_DEVICE_PTR device_ptr;

  struct timeval timeout;

  timeout.tv_sec = 1;
  timeout.tv_usec = 1;

  int socket_fd = socket_server();

  FD_ZERO(&xcept_fds);
  FD_ZERO(&read_fds);
  FD_ZERO(&write_fds);

  FD_SET(socket_fd, &read_fds);
  max_read_fd = socket_fd;

  device_ptr = get_device_root();
  while (device_ptr != NULL) {
    if (device_ptr->active) {
      FD_SET(device_ptr->serial_descriptor, &read_fds);
      if (device_ptr->serial_descriptor > max_read_fd) {
	max_read_fd = device_ptr->serial_descriptor;
      }
    }

    device_ptr = device_ptr->next;
  }

  ret_val = select(max_read_fd+1, &read_fds, &write_fds, &xcept_fds, &timeout);

  if (ret_val == -1) {
    //this is most likely from signal handling
    log_error(__FILE__, __LINE__, "select(2) failure");
    return;
  } else if (ret_val == 0) {
    //time out
    maintainer();
    return;
  } 

  // fall through to process fresh read

  device_ptr = get_device_root();
  while (device_ptr != NULL) {
    if (device_ptr->active) {
      if (FD_ISSET(device_ptr->serial_descriptor, &read_fds)) {
	device_read(device_ptr);
      }
    }

    device_ptr = device_ptr->next;
  }

  if (FD_ISSET(socket_fd, &read_fds)) {
    connect_fd = accept(socket_fd, (struct sockaddr *) NULL, (socklen_t *) NULL);
    if (connect_fd < 0) {
      log_error(__FILE__, __LINE__, "accept(2) failure");
    } else {
      responder(connect_fd);
      close(connect_fd);
    }
  }
}

/*
** test the request/response chain
** runs in foreground, arg from command line
*/
void eclectic_test(const char *arg) {
  char req_buffer[MAX_REQUEST_BUFFER];
  char res_buffer[MAX_RESPONSE_BUFFER];

  printf("--- --- --- experiment --- --- ---\n");
  printf("req:%s\n", arg);

  bzero(req_buffer, sizeof(req_buffer));
  bzero(res_buffer, sizeof(res_buffer));

  strcpy(req_buffer, arg);

  command(req_buffer, res_buffer);

  printf("res:%s\n", res_buffer);
  printf("--- --- --- experiment --- --- ---\n");
}
