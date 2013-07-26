/*
** Title:client.c
**
** Description:
**   Driver for mellow panda command line client
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
#include <unistd.h>

#include "client.h"
#include "command.h"
#include "configuration.h"
#include "log.h"
#include "socket.h"
#include "utility.h"

extern CONFIGURATION gConfiguration;

/*
**
*/
void write_diagnostics(void) {
  char buffer[MAX_RESPONSE_BUFFER];

  configuration_report(buffer);
  printf("%s\n", buffer);
}

/*
**
*/
void run_loop(char *radio, char *kommand) {
  char *cptr1;
  int max_argz;
  char *argz[MAX_ARGZ];

  char req_buffer[MAX_REQUEST_BUFFER];
  char res_buffer[MAX_RESPONSE_BUFFER];

  assert(radio != (char *) NULL);
  assert(kommand != (char *) NULL);

  max_argz = 0;
  cptr1 = strtok((char *) kommand, "/");
  while (cptr1 != NULL) {
    argz[max_argz++] = cptr1;
    cptr1 = strtok(NULL, "/");
  }

  switch(max_argz) {
  case 1:
    sprintf(req_buffer, "%s//%s/%s\n", PANDA_PROTO, kommand, radio);
    break;
  case 2:
    sprintf(req_buffer, "%s//%s/%s/%s/%s\n", PANDA_PROTO, PROTO_KEY_DEVICE, argz[0], radio, argz[1]);
    break;
  }

  printf("%s\n", req_buffer);

  socket_command(req_buffer, res_buffer);

  printf("%s\n", res_buffer);
}

/*
** 
*/
void shut_down(void) {
  log_info(__FILE__, __LINE__, "shut_down");
  log_shutdown();
}

/*
** application start
*/
BOOLEAN start_up(char *hostname, int port) {
  gConfiguration.boot_time = time(NULL);

  if (gConfiguration.write_version) {
    char temp1[64];
    panda_version_string(temp1);
    fprintf(stderr, "%s\n", temp1); 
    return(FALSE);
  }

  log_setup(CLIENT_LOG_ID);
  log_info(__FILE__, __LINE__, "start_up");

  configuration_read();

  if (hostname != NULL) {
    free(gConfiguration.server_name);
    gConfiguration.server_name = hostname;
  }
  
  if (port > 0) {
    gConfiguration.command_port = port;
  }

  if (gConfiguration.diagnostic_mode) {
    write_diagnostics();
    return(FALSE);
  }

  return(TRUE);
}

/*
** 
*/
void usage(char *arg) {
  fprintf(stderr, "usage:%s -r (radio) -k (command) [-c file][-d][-h host][-p port][-V]\n", arg);
  exit(EXIT_FAILURE);
}

/*
**
*/
int main(int argc, char *argv[]) {
  int opt;
  int port = -1;
  char *hostname = NULL;
  char *radio = NULL;
  char *kommand = NULL;

  configuration_defaults();

  while ((opt = getopt(argc, argv, "c:dh:k:p:r:x:V")) != -1) {
    switch(opt) {
    case 'c':
      fprintf(stderr, "non-standard configuration file\n");
      gConfiguration.configuration_file_name = strdup(optarg);
      break;
    case 'd':
      fprintf(stderr, "dump configuration\n");
      gConfiguration.diagnostic_mode = TRUE;
      break;
    case 'h':
      fprintf(stderr, "non-standard host\n");
      hostname = strdup(optarg);
      break;
    case 'k':
      //radio kommand
      kommand = optarg;
      break;
    case 'p':
      fprintf(stderr, "non-standard client port\n");
      port = atoi(optarg);
      break;
    case 'r':
      //radio
      radio = optarg;
      break;
    case 'V':
      //write program version and exit
      gConfiguration.write_version = TRUE;
      break;
    default:
      usage(argv[0]);
    }
  }

  if (start_up(hostname, port)) {
    if ((radio == NULL) || (kommand == NULL)) {
      usage(argv[0]);
    } else {
      run_loop(radio, kommand);
    }
  }

  shut_down();

  exit(EXIT_SUCCESS);
}
