/*
 * lock_proc.c
 * 
 * Description: Implements kernel mode callbacks for process locking.
 */

#include "lock_proc.h"

/* Holds locked processes and callbacks. */
SINGLE_LIST_ENTRY* lock_proc_list = NULL;
PVOID callbacks = NULL;

typedef struct lock_proc_entry {
        HANDLE pid;
        SINGLE_LIST_ENTRY next;
} lock_proc_entry;

NTSTATUS lock_proc(HANDLE pid)
{
        if (lock_proc_list == NULL) {
                lock_proc_entry* curr = (lock_proc_entry*)
                        ExAllocatePoolWithTag(PagedPool,
                                sizeof(lock_proc_entry), 0x1337);
                
                if (curr != NULL) {
                        curr->pid = pid;
                        lock_proc_list = &curr->next;
                }
        }
        else {
                lock_proc_entry* curr = (lock_proc_entry*)
                        ExAllocatePoolWithTag(PagedPool,
                                sizeof(lock_proc_entry), 0x1337);

                if (curr != NULL) {
                        curr->pid = pid;
                        PushEntryList(lock_proc_list, &curr->next);
                }
        }

        return STATUS_SUCCESS;
}

OB_PREOP_CALLBACK_STATUS obj_pre_callback(PVOID RegistrationContext,
        POB_PRE_OPERATION_INFORMATION op_info)
{
        UNREFERENCED_PARAMETER(RegistrationContext);

        if (op_info->ObjectType == *PsProcessType &&
                op_info->Object != PsGetCurrentProcess()) {

                SINGLE_LIST_ENTRY* curr = lock_proc_list;
                while (curr != NULL) {
                        HANDLE pid = NULL;
                        if (op_info->Operation != 0)
                                pid = PsGetProcessId((PEPROCESS)
                                        op_info->Operation);

                        if (CONTAINING_RECORD(curr, lock_proc_entry, next)->pid
                                == pid && op_info->Operation
                                == OB_OPERATION_HANDLE_CREATE && 
                                op_info->KernelHandle != TRUE)
                                op_info->Parameters->
                                CreateHandleInformation.DesiredAccess
                                &= PROCESS_TERMINATE;

                        curr = curr->Next;
                }
        }
        else if (op_info->ObjectType == *PsThreadType) {
                HANDLE pid = PsGetThreadProcessId((PETHREAD)op_info->Object);
                if (pid != PsGetCurrentProcessId())
                        return OB_PREOP_SUCCESS;

                SINGLE_LIST_ENTRY* curr = lock_proc_list;
                while (curr != NULL) {
                        if (CONTAINING_RECORD(curr, lock_proc_entry, next)->pid
                                == pid && op_info->Operation
                                == OB_OPERATION_HANDLE_CREATE &&
                                op_info->KernelHandle != TRUE)
                                op_info->Parameters->
                                CreateHandleInformation.DesiredAccess
                                &= THREAD_TERMINATE;

                        curr = curr->Next;
                }
        }

        return OB_PREOP_SUCCESS;
}

NTSTATUS reg_proc_callback()
{
        OB_CALLBACK_REGISTRATION ob_reg;
        OB_OPERATION_REGISTRATION op_reg[2] = {NULL};

        op_reg[0].ObjectType = PsProcessType;
        op_reg[0].Operations |= OB_OPERATION_HANDLE_CREATE;
        op_reg[0].PreOperation = obj_pre_callback;
        op_reg[0].PostOperation = NULL;

        op_reg[1].ObjectType = PsProcessType;
        op_reg[1].Operations |= OB_OPERATION_HANDLE_CREATE;
        op_reg[1].PreOperation = obj_pre_callback;
        op_reg[1].PostOperation = NULL;

        UNICODE_STRING altitude;
        RtlInitUnicodeString(&altitude, L"1234567890");
        ob_reg.Version = OB_FLT_REGISTRATION_VERSION;
        ob_reg.OperationRegistrationCount = 2;
        ob_reg.Altitude = altitude;
        ob_reg.RegistrationContext = NULL;
        ob_reg.OperationRegistration = op_reg;

        return ObRegisterCallbacks(&ob_reg, callbacks);
}

NTSTATUS free_proc_filter()
{
        if (callbacks != NULL) {
                ObUnRegisterCallbacks(callbacks);
                callbacks = NULL;
        }

        return STATUS_SUCCESS;
}