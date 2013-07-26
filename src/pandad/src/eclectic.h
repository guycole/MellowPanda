/*
** Title:eclectic.h
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
**   $Id$
**
**   $Log$
*/

#ifndef __ECLECTIC_H__
#define __ECLECTIC_H__

/*
** poll socket via select(2) for new command or timeout
*/
extern void eclectic(void);

/*
** test the request/response chain
** runs in foreground, arg from command line
*/
extern void eclectic_test(const char *arg);

#endif /* __ECLECTIC_H__ */
