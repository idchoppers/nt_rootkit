/*
 * lock_key.c
 *
 * Description: Implements registry key locking.
 */

#include "lock_key.h"

/* Holds locked reg keys and callbacks. */
LIST_ENTRY* lock_key_list = NULL;
PLARGE_INTEGER cookie = NULL;

typedef struct lock_key_entry {
        PUNICODE_STRING key;
        LIST_ENTRY list_entry;
} lock_key_entry;

NTSTATUS lock_key(PUNICODE_STRING key) {
        lock_key_entry key_entry;
        key_entry.key = key;
        InsertTailList(lock_key_list, &key_entry.list_entry);

        return STATUS_SUCCESS;
}

NTSTATUS on_reg_notify(PVOID CallbackContext, PVOID Argument1, PVOID Argument2)
{
        UNREFERENCED_PARAMETER(CallbackContext);
        NTSTATUS status = STATUS_SUCCESS;

        switch ((REG_NOTIFY_CLASS)(ULONG_PTR)Argument1) {
        case RegNtPreSetValueKey:
                if (Argument2 == NULL)
                        break;

                PREG_SET_VALUE_KEY_INFORMATION pre_info =
                        (PREG_SET_VALUE_KEY_INFORMATION)Argument2;

                PUNICODE_STRING key_name = NULL;
                if (!NT_SUCCESS(CmCallbackGetKeyObjectID(cookie,
                        pre_info->Object, NULL, &key_name)))
                        break;

                for (LIST_ENTRY* i = lock_key_list; i != lock_key_list; ++i) {
                        PLIST_ENTRY entry = RemoveHeadList(lock_key_list);
                        lock_key_entry* info = CONTAINING_RECORD(
                                entry, lock_key_entry, key);
                        PCWSTR k_name = (PCWSTR)info->key;

                        UNICODE_STRING cmp_name;
                        RtlInitUnicodeString(&cmp_name, k_name);

                        if (RtlCompareUnicodeString(key_name, &cmp_name, TRUE)
                                == 0) {
                                InsertTailList(lock_key_list, entry);
                                status = STATUS_CALLBACK_BYPASS;
                                break;
                        }
                        InsertTailList(lock_key_list, entry);
                }
        }

        return status;
}

NTSTATUS reg_key_callback(PDRIVER_OBJECT driver_obj)
{
        UNICODE_STRING altitude;
        RtlInitUnicodeString(&altitude, L"1234567890");

        return CmRegisterCallbackEx(on_reg_notify, &altitude,
                driver_obj, NULL, cookie, NULL);
}

NTSTATUS free_key_filter()
{
        return CmUnRegisterCallback(*cookie);
}