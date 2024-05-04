#include "lock_key.h"
#include "ioctl.h"

NTSTATUS on_reg_notify(PVOID CallbackContext, PVOID Argument1, PVOID Argument2)
{
        NTSTATUS status = STATUS_SUCCESS;

        switch ((REG_NOTIFY_CLASS)(ULONG_PTR)Argument1) {
        case RegNtPreSetValueKey:
        {
                if (Argument2 == NULL)
                        break;

                PREG_SET_VALUE_KEY_INFORMATION pre_info =
                        (PREG_SET_VALUE_KEY_INFORMATION)Argument2;

                PUNICODE_STRING key_name = NULL;
                if (!NT_SUCCESS(CmCallbackGetKeyObjectID(&cookie,
                        pre_info->Object, NULL, &key_name)))
                        break;

                // TODO Needs bugfixed
                for (LIST_ENTRY* i = lock_key_list; i != lock_key_list; ++i) {
                        auto entry = RemoveHeadList(&lock_key_list);
                        auto info = CONTAINING_RECORD(entry,
                                lock_key_entry, key);
                        auto k_name = (WCHAR*)&info->key;

                        UNICODE_STRING cmp_name;
                        RtlInitUnicodeString(&cmp_name, k_name);

                        if (RtlCompareUnicodeString(key_name, &cmp_name, TRUE)
                                == 0) {
                                InsertTailList(&lock_key_list, entry);
                                status = STATUS_CALLBACK_BYPASS;
                                break;
                        }
                        InsertTailList(&lock_key_list, entry);
                }
        }
        }

        return status;
}

NTSTATUS reg_key_callback(PDRIVER_OBJECT driver_obj)
{
        UNICODE_STRING altitude;
        RtlInitUnicodeString(&altitude, L"1234567890");

        NTSTATUS status = CmRegisterCallbackEx(on_reg_notify, &altitude,
                driver_obj, NULL, &cookie, NULL);
        if (!NT_SUCCESS(status))
                //DbgPrint(("failed to get reg key callback: %08X\n", status));

        return status;
}

NTSTATUS free_key_filter()
{
        NTSTATUS status = CmUnRegisterCallback(*cookie);
        if (!NT_SUCCESS(status))
                //DbgPrint(("failed to unreg callbacks: %08X\n", status));
        return status;
}