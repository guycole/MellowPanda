/*
** Title:rs232.h
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
**   $Id$
**
**   $Log$
*/

#ifndef __RS232_H__
#define __RS232_H__

#define MAX_RX_BUFFER 1024

#define TTY_S0 "/dev/ttyS0"
#define TTY_S1 "/dev/ttyS1"
#define TTY_USB_S0 "/dev/ttyUSB0"
#define TTY_USB_S1 "/dev/ttyUSB1"

#define TEST_MSG1 "the quick brown fox jumped over the lazy dogs back"
#define TEST_MSG2 "RYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRYRY"
#define TEST_MSG3 "of all the fishies in the sea the mermaid is the one for me"

/*
** Open specified serial port.
** Returns file descriptor if success or -1 failure
*/
extern int open_port(const char *port, const char *speed, const char *paramz);

/*
** Close specified serial port.
*/
extern void close_port(int fd);

/*
** Read the specified port.  Does not block.
** Returns read count (with populated buffer).
*/
extern int read_port(int fd, char *buffer);

/*
** Write to specified port.
** Returns write count or -1 if failure.
*/
extern int write_port(int fd, char delimiter, const char *message);

#endif /* __RS232_H__ */
