#ifndef _LOCK_KEY_H_
#define _LOCK_KEY_H_

#include <ntddk.h>

LIST_ENTRY* lock_key_list = NULL;
PLARGE_INTEGER cookie = NULL;

typedef struct lock_key_entry {
        PUNICODE_STRING key;
} lock_key_entry;

EX_CALLBACK_FUNCTION on_reg_notify;

NTSTATUS on_reg_notify(PVOID CallbackContext, PVOID Argument1, PVOID Argument2);

NTSTATUS reg_key_callback(PDRIVER_OBJECT driver_obj);

NTSTATUS free_key_filter();

#endif