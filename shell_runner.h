#ifndef SHELL_RUNNER_H
#define SHELL_RUNNER_H

#define SHELL_RUNNER_DBUS_SERVICE_NAME      "com.example.MyService"
#define SHELL_RUNNER_DBUS_OBJECT_PATH       "/com/example/MyService"
#define SHELL_RUNNER_DBUS_INTERFACE_NAME    "com.example.MyInterface"

int run_dbus_command(char* command, char** result, int *exitcode);

#endif /* SHELL_RUNNER_H */
