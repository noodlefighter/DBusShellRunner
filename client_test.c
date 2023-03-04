#include <stdio.h>
#include <stdlib.h>
#include "shell_runner.h"

int main()
{
    char* output;
    int exitcode;
    int ret = run_dbus_command("ls /", &output, &exitcode);
    if (ret == 0) {
        printf("DBus Command succeeded:\nexitcode:%d\n%s", exitcode, output);
    }
    else if (ret != 0) {
        printf("DBus Command failed, return %d\n", ret);
    }
    free(output);
    return 0;
}
