#include <stdio.h>
#include <stdlib.h>
#include "shell_runner.h"

int main(int argc, char **argv)
{
    std::string output;
    int exitcode;
    if (shell_runner_exec("ls /", output, exitcode)) {
        printf("DBus Command succeeded:\nexitcode:%d\n%s", exitcode, output);
    }
    else {
        printf("DBus Command failed\n");
    }
    return 0;
}
