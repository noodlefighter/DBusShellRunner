#include <stdio.h>
#include <stdlib.h>
#include "shell_runner.h"

int main()
{
    char* output = NULL;
    int exitcode;
    int ret = shell_runner_exec("ls /", &output, &exitcode);
    if (ret == 0) {
        printf("DBus Command succeeded:\nexitcode:%d\n%s", exitcode, output);
    }
    else if (ret != 0) {
        printf("DBus Command failed, return %d\n", ret);
    }
    if (output) {
        free(output);
        output = NULL;
    }
    return 0;
}
