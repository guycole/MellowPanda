/*
** Title:socket.c
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
**   $Log$
*/
static char rcsid[] = "$Id$";

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "command.h"
#include "configuration.h"
#include "log.h"
#include "socket.h"

extern CONFIGURATION gConfiguration;

/*
** establish the command socket and return associated file descriptor
*/
int socket_client() {
  unsigned length;
  int socket_fd;
  struct hostent *host_ptr;
  struct sockaddr_in address;
  char log_message[MAX_LOG_MESSAGE];
  
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket open failure");
    log_fatal(__FILE__, __LINE__, "socket open failure");
  }

  host_ptr = gethostbyname(gConfiguration.server_name);
  if (host_ptr == NULL) {
    sprintf(log_message, "server lookup failure:%s", gConfiguration.server_name);
    log_fatal(__FILE__, __LINE__, log_message);
  }
  
  bzero((char *) &address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_port = htons(gConfiguration.command_port);
  //  address.sin_addr.s_addr = htonl(INADDR_ANY);
  bcopy((char *) host_ptr->h_addr, (char *)&address.sin_addr, host_ptr->h_length);
  
  if (connect(socket_fd, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) < 0) {
    close(socket_fd);
    log_fatal(__FILE__, __LINE__, "connect failure");
  }

  length = sizeof(struct sockaddr_in);
  if (getsockname(socket_fd, (struct sockaddr *) &address, &length) < 0) {
    perror("socket name failure");
    log_fatal(__FILE__, __LINE__, "socket name failure");
  }

  sprintf(log_message, "open port:%d fd:%d", ntohs(address.sin_port), socket_fd);
  log_info(__FILE__, __LINE__, log_message);

  return(socket_fd);
}

/*
** given a file descriptor, return a message
*/
void socket_reader(int fd, char *message) {
  BOOLEAN flag;
  char cc;
  int rc, ndx;
  char buffer[MAX_REQUEST_BUFFER];

  assert(message != (char *) NULL);

  bzero(buffer, MAX_REQUEST_BUFFER);

  for (ndx = 0, flag = TRUE; ((ndx < MAX_REQUEST_BUFFER) && (flag)); ndx++) {
    rc = read(fd, &cc, 1);

    switch(rc) {
    case 0:
      log_debug(__FILE__, __LINE__, "remote connection closed");
      flag = FALSE;
      break;
    case 1:
      if (cc == '\n') {
	flag = FALSE;
      } else {
	buffer[ndx] = cc;
      }
      break;
    default:
      log_error(__FILE__, __LINE__, "read(2) failure");
      return;
    }
  }

  strcpy(message, buffer);
}

/*
** write a command to the server and obtain a response
** request must be terminated w/a newline
*/
void socket_command(char *request, char *response) {
  int fd;

  assert(request != (char *) NULL);
  assert(response != (char *) NULL);

  log_debug(__FILE__, __LINE__, request);

  fd = socket_client();
  if (fd < 0) {
    log_error(__FILE__, __LINE__, "bad socket");
    return;
  }

  if (write(fd, request, strlen(request)) < 0) {
    log_error(__FILE__, __LINE__, "write failure");
  }

  socket_reader(fd, response);

  close(fd);

  log_debug(__FILE__, __LINE__, response);
}

/*
** establish the server command socket and return associated file descriptor
** any socket creation error is fatal
*/
int socket_server(void) {
  unsigned length;
  int socket_fd;
  struct hostent *host_ptr;
  struct sockaddr_in address;
  char log_message[MAX_LOG_MESSAGE];

  if (gConfiguration.command_fd > 0) {
    //    log_info(__FILE__, __LINE__, "socket already created");
    return(gConfiguration.command_fd);
  }

  // fall through for creation

  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket open failure");
    log_fatal(__FILE__, __LINE__, "socket open failure");
  }

  host_ptr = gethostbyname(gConfiguration.server_name);
  if (host_ptr == NULL) {
    sprintf(log_message, "server lookup failure:%s", gConfiguration.server_name);
    log_fatal(__FILE__, __LINE__, log_message);
  }

  bzero((char *) &address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_port = htons(gConfiguration.command_port);
  bcopy((char *) host_ptr->h_addr, (char *)&address.sin_addr, host_ptr->h_length);
  
  if (bind(socket_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
    perror("socket bind failure");
    log_fatal(__FILE__, __LINE__, "socket bind failure");
  }

  if (listen(socket_fd, MAX_SOCKET_BACKLOG) < 0) {
    perror("listen failure");
    log_fatal(__FILE__, __LINE__, "listen failure");
  }

  length = sizeof(struct sockaddr_in);
  if (getsockname(socket_fd, (struct sockaddr *) &address, &length) < 0) {
    perror("socket name failure");
    log_fatal(__FILE__, __LINE__, "socket name failure");
  }

  sprintf(log_message, "open port:%d fd:%d", ntohs(address.sin_port), socket_fd);
  log_info(__FILE__, __LINE__, log_message);

  return(socket_fd);
}

/*
** establish a broadcast update UDP socket
*/
int socket_update_create(SERIAL_DEVICE_PTR device) {
  int broadcast = 1;
  int socket_fd;
  struct sockaddr_in address;
  char log_message[MAX_LOG_MESSAGE];

  if (device->update_descriptor > 0) {
    log_info(__FILE__, __LINE__, "update socket already created");
    return(device->update_descriptor);
  }

  // fall through for creation

  if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    perror("socket open failure");
    log_fatal(__FILE__, __LINE__, "socket open failure");
  }

  if (setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
    perror("socket option failure");
    log_fatal(__FILE__, __LINE__, "socket option failure");
  }

  sprintf(log_message, "open update port:%d fd:%d", ntohs(address.sin_port), socket_fd);
  log_info(__FILE__, __LINE__, log_message);

  return(socket_fd);
}

/*
** write broadcast message
*/
void socket_update_write(SERIAL_DEVICE_PTR device) {
  struct hostent *host_ptr;
  struct sockaddr_in address;
  char log_message[MAX_LOG_MESSAGE];

  if (device->update_descriptor < 0) {
    log_info(__FILE__, __LINE__, "broadcast write w/bad file descriptor");
    return;
  }

  if ((!strcmp(gConfiguration.server_name, "localhost")) || (!strcmp(gConfiguration.server_name, "127.0.0.1"))) {
    host_ptr = gethostbyname("127.0.0.255");
  } else {
    host_ptr = gethostbyname("255.255.255.255");
  }

  if (host_ptr == NULL) {
    sprintf(log_message, "server lookup failure:%s", gConfiguration.server_name);
    log_fatal(__FILE__, __LINE__, log_message);
  }

  bzero((char *) &address, sizeof(address));

  address.sin_family = AF_INET;
  address.sin_port = htons(device->update_port);
  bcopy((char *) host_ptr->h_addr, (char *)&address.sin_addr, host_ptr->h_length);

  if (sendto(device->update_descriptor, device->rx_buffer, strlen(device->rx_buffer), 0, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("sending datagram message");
    sprintf(log_message, "error sending datagram:%d:%s:%s", device->update_descriptor, device->name, device->rx_buffer);
    log_error(__FILE__, __LINE__, log_message);
  }
}
