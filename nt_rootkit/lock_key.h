/*
 * lock_key.h
 * 
 * Description: Defines the functions that allow for Registry Key locking.
 */

#ifndef _LOCK_KEY_H_
#define _LOCK_KEY_H_

#include <ntddk.h>
#include "ioctl.h"

EX_CALLBACK_FUNCTION on_reg_notify;

NTSTATUS on_reg_notify(PVOID CallbackContext, PVOID Argument1, PVOID Argument2);

NTSTATUS reg_key_callback(PDRIVER_OBJECT driver_obj);

NTSTATUS free_key_filter();

#endif