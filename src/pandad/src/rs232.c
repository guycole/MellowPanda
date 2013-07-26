/*
** Title:rs232.c
**
** Description:
**   Generic RS-232 routines.
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

#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <termios.h>

#include "log.h"
#include "rs232.h"
#include "utility.h"

/*
** Open specified serial port.
** Returns file descriptor if success or -1 failure
*/
int open_port(const char *port, const char *speed, const char *paramz) {
  int fd;

  speed_t baud_rate;
  struct termios termioz;
  char log_message[MAX_LOG_MESSAGE];

  assert(port != (char *) NULL);
  assert(speed != (char *) NULL);
  assert(paramz != (char *) NULL);

  if (!strcmp("B1200", speed)) {
    baud_rate = B1200;
  } else if (!strcmp("B2400", speed)) {
    baud_rate = B2400;
  } else if (!strcmp("B4800", speed)) {
    baud_rate = B4800;
  } else if (!strcmp("B9600", speed)) {
    baud_rate = B9600;
  } else if (!strcmp("B19200", speed)) {
    baud_rate = B19200;
  } else if (!strcmp("B38400", speed)) {
    baud_rate = B38400;
  } else if (!strcmp("B57600", speed)) {
    baud_rate = B57600;
  } else if (!strcmp("B115200", speed)) {
    baud_rate = B115200;
  } else if (!strcmp("B230400", speed)) {
    baud_rate = B230400;
  } else {
    baud_rate = B9600;

    sprintf(log_message, "unknown speed:%s using 9600 baud", speed);
    log_error(__FILE__, __LINE__, log_message);
  }

  fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1) {
    sprintf(log_message, "open_port failure:%s", port);
    log_error(__FILE__, __LINE__, log_message);

    perror(log_message);

    return(-1);
  }

  fcntl(fd, F_SETFL, FNDELAY); /* non-blocking */
  //fcntl(fd, F_SETFL, 0); /* blocking */

  tcgetattr(fd, &termioz);

  cfsetispeed(&termioz, baud_rate);
  cfsetospeed(&termioz, baud_rate);

  /* always 8N1 */
  termioz.c_cflag &= ~PARENB;
  termioz.c_cflag &= ~CSTOPB;
  termioz.c_cflag &= ~CSIZE; 
  termioz.c_cflag |= CS8;

  termioz.c_cflag |= (CLOCAL | CREAD);
  termioz.c_cflag &= ~CRTSCTS; /* disable hardware flow control */
  
  termioz.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  
  tcsetattr(fd, TCSAFLUSH, &termioz);

  return(fd);
}

/*
** Close specified serial port.
*/
void close_port(int fd) {
  close(fd);
}

/*
** Read the specified port.  
** Returns read count (with populated buffer).
*/
int read_port(int fd, char *buffer) {
  int rc;
  int finished;
  int wait_counter = 13;

  struct timespec remain_time;
  struct timespec sleep_time;

  int ndx;
  char cc;
  char data[MAX_RESPONSE_BUFFER];
  char log_message[MAX_LOG_MESSAGE];

  assert(buffer != (char *) NULL);

  bzero(data, MAX_RESPONSE_BUFFER);

  sleep_time.tv_sec = 0;
  sleep_time.tv_nsec = 10000000;

  ndx = 0;
  finished = 0;
  while ((!finished) && (wait_counter > 0) && (ndx < MAX_RESPONSE_BUFFER)) {
    rc = read(fd, &cc, 1);

    if (rc < 0) {
      nanosleep(&sleep_time, &remain_time);
      --wait_counter;
    } else {
      wait_counter = 10;

      //      printf("%c/%x ", cc, cc);

      if (cc == '\n') { //termios maps cr to lf
	finished = 1;
      } else {
	data[ndx] = cc;
	ndx++;
      }
    }
  }

#if 0
  if (wait_counter < 1) {
    printf("...rs232 timeout...%d\n", ndx);
    log_debug(__FILE__, __LINE__, "...rs232 timeout...");
  }
#endif

  if (ndx >= MAX_RESPONSE_BUFFER) {
    printf("...rs232 full buffer...\n");
    log_debug(__FILE__, __LINE__, "...rs232 full buffer...");
  }

  sprintf(log_message, "read_port:%d:%d:%s", fd, strlen(data), data);
  //  log_debug(__FILE__, __LINE__, log_message);

  strcpy(buffer, data);

  return(ndx);
}

/*
** Write to specified port.
** Returns write count or -1 if failure.
*/
int write_port(int fd, char delimiter, const char *message) {
  ssize_t counter, limit;
  char buffer[MAX_RESPONSE_BUFFER];
  char log_message[MAX_LOG_MESSAGE];

  assert(message != (char *) NULL);

  sprintf(log_message, "write_port:%d:%s", fd, message);
  log_debug(__FILE__, __LINE__, log_message);

  sprintf(buffer, "%s%c", message, delimiter);

  limit = strlen(buffer);
  counter = write(fd, buffer, limit);
  
  if (counter < 0) {
    sprintf(log_message, "write_port failure:%d:%s", fd, message);
    log_error(__FILE__, __LINE__, log_message);

    perror("write_port failure");
  }

  return(counter);
}
