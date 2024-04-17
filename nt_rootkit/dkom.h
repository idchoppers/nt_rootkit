/*
        dkom.h

        Direct kernel object manipulation functions.

        TDOD make params for proc operations use HANDLE inputs!
*/

#ifndef _DKOM_H_
#define _DKOM_H_

#include <ntddk.h>

/*
        Unlinks a LIST_ENTRY field in an EPROCESS
        from the process list.
*/
NTSTATUS proc_unlink(UINT32 pid);

/*
        Changes an EPROCESS's UniqueProcessId.
*/
NTSTATUS proc_set_pid(UINT32 pid);

#endif