/*
** Title:utility.h
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
**   $Id$
**
**   $Log$
*/

#ifndef __UTILITY_H__
#define __UTILITY_H__

/*
**
*/
#define MAX_REQUEST_BUFFER 2048
#define MAX_RESPONSE_BUFFER 4096

/*
** employed to map strings to enumerated type (and back again)
*/
#define BC245_STRING "BC245"
#define BC780_STRING "BC780"
#define BC895_STRING "BC895"
#define PCR1000_STRING "PCR1000"
#define PCR1500_STRING "PCR1500"
#define PRO2052_STRING "PRO2052"
#define R71_STRING "R71"
#define R7000_STRING "R7000"
#define RAW_DEVICE_STRING "RAW_DEVICE"

/*
** employed to map strings to enumerated commands (and back again)
*/
#define PING_COMMAND_STRING "PING"
#define RAW_COMMAND_STRING "RAW"
#define TUNE_COMMAND_STRING "TUNE"
#define UNKNOWN_COMMAND_STRING "UNKNOWN"

/*
**
*/
typedef enum {
  FALSE, TRUE
} BOOLEAN;

/*
** legal device types (configuration file and beyond)
*/
typedef enum {
  UNKNOWN_DEVICE, RAW_DEVICE, PCR1000, PCR1500, R71, R7000, BC895, BC245, BC780, PRO2052
} SERIAL_DEVICE_TYPE;

/*
** legal command options
*/
typedef enum {
  UNKNOWN_COMMAND, RAW_COMMAND, PING_COMMAND, TUNE_COMMAND
} SERIAL_COMMAND_TYPE;

/*
** delimit the end of a command/response
*/
typedef enum {
  UNDEFINED_DELIMITER, CR_DELIMITER, LF_DELIMITER
} SERIAL_COMMAND_DELIMITER;

/*
** map enumerated command from string
*/
extern SERIAL_COMMAND_TYPE utility_map_serial_command(const char *candidate);

/*
** map enumerated type from string
*/
extern SERIAL_DEVICE_TYPE utility_map_device_type(const char *candidate);

/*
** map enumerated type to string
*/
extern void utility_map_device_type_to_string(SERIAL_DEVICE_TYPE candidate, char *result);

/*
** map enumerated type from string
*/
extern BOOLEAN utility_map_boolean(const char *candidate);

/*
** map enumerated type from string
*/
extern SERIAL_COMMAND_DELIMITER utility_map_delimiter(const char *candidate);

/*
** map enumerated type to string
*/
extern void utility_map_delimiter_to_string(SERIAL_DEVICE_TYPE candidate, char *result);

/*
** return delimiter value
*/
extern char utility_delimiter(SERIAL_COMMAND_DELIMITER arg);

/*
** return server version string
*/
extern void pandad_version_string(char *result);

/*
** return client version string
*/
extern void panda_version_string(char *result);

#endif /* __UTILITY_H__ */
