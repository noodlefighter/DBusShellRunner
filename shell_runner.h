#ifndef SHELL_RUNNER_H
#define SHELL_RUNNER_H

#define SHELL_RUNNER_DBUS_SERVICE_NAME      "com.example.MyService"
#define SHELL_RUNNER_DBUS_OBJECT_PATH       "/com/example/MyService"
#define SHELL_RUNNER_DBUS_INTERFACE_NAME    "com.example.MyInterface"

#ifdef __cplusplus
extern "C" {
#endif

int shell_runner_exec(const char* command, char** result, int *exitcode);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
#include <string>
static inline
bool shell_runner_exec(const char *command, std::string &result, int &exitcode) {
    char *result_c = NULL;
    int ret = shell_runner_exec(command, &result_c, &exitcode);
    if (0 == ret) {
        if (NULL != result_c) {
            result = result_c;
            free(result_c);
        }
        return true;
    }
    else {
        return false;
    }
}
#endif

#endif /* SHELL_RUNNER_H */
