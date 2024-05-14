/*
 * ioctl.h
 *
 * Description: Defines IRP and driver object controls.
 */

#ifndef _IOCTL_H_
#define _IOCTL_H_

#include <ntddk.h>

#define DRIVER_PREFIX   (0x1337)

NTSTATUS nt_rootkit_create_close(PDEVICE_OBJECT driver_obj, PIRP irp);

NTSTATUS nt_rootkit_read(PDEVICE_OBJECT driver_obj);

NTSTATUS nt_rootkit_write(PDEVICE_OBJECT driver_obj);

#endif