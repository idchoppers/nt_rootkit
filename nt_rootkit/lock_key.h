#ifndef _LOCK_KEY_H_
#define _LOCK_KEY_H_

#include <ntddk.h>

NTSTATUS on_reg_notify(PVOID arg0, PVOID arg1);

NTSTATUS reg_key_callback();

NTSTATUS free_key_filter();

#endif