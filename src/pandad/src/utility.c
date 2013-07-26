/*
** Title:utility.c
**
** Description:
**   General purpose helper routines.
**   Utility should not refer to any other Mellow Panda routines.
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
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

#include "utility.h"

/*
** map enumerated command from string
*/
SERIAL_COMMAND_TYPE utility_map_serial_command(const char *candidate) {
  assert(candidate != (char *) NULL);

  if (!strncasecmp(candidate, "ping", strlen("ping"))) {
    return(PING_COMMAND);
  } else if (!strncasecmp(candidate, "raw", strlen("raw"))) {
    return(RAW_COMMAND);
  } else if (!strncasecmp(candidate, "tune", strlen("tune"))) {
    return(TUNE_COMMAND);
  }

  return(UNKNOWN_COMMAND);
}

/*
** map enumerated type from string
*/
SERIAL_DEVICE_TYPE utility_map_device_type(const char *candidate) {
  assert(candidate != (char *) NULL);

  if (!strncasecmp(candidate, "RAW_DEVICE", strlen("RAW_DEVICE"))) {
    return(RAW_DEVICE);
  } else if (!strncasecmp(candidate, "PCR1000", strlen("PCR1000"))) {
    return(PCR1000);
  } else if (!strncasecmp(candidate, "PCR1500", strlen("PRC1500"))) {
    return(PCR1500);
  } else if (!strncasecmp(candidate, "R71", strlen("R71"))) {
    return(R71);
  } else if (!strncasecmp(candidate, "R7000", strlen("R7000"))) {
    return(R7000);
  } else if (!strncasecmp(candidate, "BC895", strlen("BC895"))) {
    return(BC895);
  } else if (!strncasecmp(candidate, "BC245", strlen("BC245"))) {
    return(BC245);
  } else if (!strncasecmp(candidate, "BC780", strlen("BC780"))) {
    return(BC780);
  } else if (!strncasecmp(candidate, "PRO2052", strlen("PRO2052"))) {
    return(PRO2052);
  }

  return(UNKNOWN_DEVICE);
}

/*
** map enumerated type to string
*/
void utility_map_device_type_to_string(SERIAL_DEVICE_TYPE candidate, char *result) {
  switch(candidate) {
  case RAW_DEVICE:
    strcpy(result, "RawDevice");
    break;
  case PCR1000:
    strcpy(result, "PCR1000");
    break;
  case PCR1500:
    strcpy(result, "PCR1500");
    break;
  case R71:
    strcpy(result, "R71");
    break;
  case R7000:
    strcpy(result, "R7000");
    break;
  case BC895:
    strcpy(result, "BC895");
    break;
  case BC245:
    strcpy(result, "BC245");
    break;
  case BC780:
    strcpy(result, "BC780");
    break;
  case PRO2052:
    strcpy(result, "PRO2052");
    break;
  default:
    strcpy(result, "Unknown");
  }
}

/*
** map enumerated type from string
*/
BOOLEAN utility_map_boolean(const char *candidate) {
  assert(candidate != (char *) NULL);

  if (!strcasecmp("yes", candidate)) {
    return(TRUE);
  } else if (!strcasecmp("true", candidate)) {
    return(TRUE);
  }

  return(FALSE);
}

/*
** map enumerated type from string
*/
SERIAL_COMMAND_DELIMITER utility_map_delimiter(const char *candidate) {
  assert(candidate != (char *) NULL);

  if (!strcasecmp("cr", candidate)) {
    return(CR_DELIMITER);
  } else if (!strcasecmp("lf", candidate)) {
    return(LF_DELIMITER);
  }

  return(UNDEFINED_DELIMITER);
}

/*
** return delimiter value
*/
char utility_delimiter(SERIAL_COMMAND_DELIMITER arg) {
  switch(arg) {
  case CR_DELIMITER:
    return('\r');
  case LF_DELIMITER:
    return('\n');
  default:
    return('\0');
  }
}

/*
** map enumerated type to string
*/
void utility_map_delimiter_to_string(SERIAL_DEVICE_TYPE candidate, char *result) {
  switch(candidate) {
  case CR_DELIMITER:
    strcpy(result, "CR");
    break;
  case LF_DELIMITER:
    strcpy(result, "LF");
    break;
  case UNDEFINED_DELIMITER:
    strcpy(result, "UNDEF");
    break;
  default:
    strcpy(result, "Unknown");
  }
}

/*
** return server version string
*/
void pandad_version_string(char *result) {
  sprintf(result, "pandad %s compiled on %s at %s", VERSION, __DATE__, __TIME__); 
}

/*
** return client version string
*/
void panda_version_string(char *result) {
  sprintf(result, "panda %s compiled on %s at %s", VERSION, __DATE__, __TIME__); 
}
