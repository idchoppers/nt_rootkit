#include <stdio.h>
#include <Windows.h>
#include "ioctl_globals.h"

int main(int argc, char* argv[], char* envp[])
{
        BOOL status = FALSE;

        if (argc > 1) {
                switch (*argv[1]) {
                case 0:
                        if (*argv[2] > 0) {

                        }
                        break;
                case 1:
                        if (*argv[2] > 0) {

                        }
                        break;
                case 2:
                        if (*argv[2] > 0) {

                        }
                        break;
                case 3:
                        if (RtlUnicodeStringValidate(argv[2])) {

                        }
                        break;
                }
        }

        return status;
}