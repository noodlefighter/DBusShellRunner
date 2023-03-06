#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dbus/dbus.h>
#include "shell_runner.h"

#define DBUS_SERVICE_NAME    SHELL_RUNNER_DBUS_SERVICE_NAME
#define DBUS_OBJECT_PATH     SHELL_RUNNER_DBUS_OBJECT_PATH
#define DBUS_INTERFACE_NAME  SHELL_RUNNER_DBUS_INTERFACE_NAME

char *ExecuteShellCommandWithExitCode(const char *command, int *exit_code) {
    FILE *fp;
    char buffer[1024];
    char *output = malloc(1);
    size_t output_size = 0;
    int status;

    fp = popen(command, "r");
    if (fp == NULL) {
        *exit_code = -1;
        return NULL;
    }

    output[0] = '\0';
    memset(buffer, 0, sizeof(buffer));

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        output_size += strlen(buffer);
        output = realloc(output, output_size + 1);
        strcat(output, buffer);
    }

    status = pclose(fp);
    if (WIFEXITED(status)) {
        *exit_code = WEXITSTATUS(status);
    } else {
        *exit_code = -1;
    }

    return output;
}

static DBusHandlerResult dbus_request_handler(DBusConnection *conn, DBusMessage *message, void *user_data)
{
    DBusMessage *reply = NULL;
    DBusError error;
    dbus_error_init(&error);

    const char *command = NULL;
    if (!dbus_message_get_args(message, &error, DBUS_TYPE_STRING, &command, DBUS_TYPE_INVALID)) {
        reply = dbus_message_new_error(message, error.name, error.message);
        dbus_error_free(&error);
    } else {
        char *output = NULL;
        int exit_code = 0;
        output = ExecuteShellCommandWithExitCode(command, &exit_code);
        printf("Exit code: %d\n%s", exit_code, output);
        reply = dbus_message_new_method_return(message);
        DBusMessageIter iter;
        dbus_message_iter_init_append(reply, &iter);
        dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &output);
        dbus_message_iter_append_basic(&iter, DBUS_TYPE_INT32, &exit_code);

        if (NULL != output) {
            free(output);
        }
    }

    if (!dbus_connection_send(conn, reply, NULL)) {
        fprintf(stderr, "Failed to send DBus reply.\n");
    }

    dbus_message_unref(reply);

    return DBUS_HANDLER_RESULT_HANDLED;
}

int main(int argc, char **argv) {
    DBusError err;
    dbus_error_init(&err);

    DBusConnection *conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "DBus connection error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }

    dbus_bool_t ret = dbus_bus_request_name(conn, DBUS_SERVICE_NAME, DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    if (dbus_error_is_set(&err)) {
        fprintf(stderr, "Name request error: %s\n", err.message);
        dbus_error_free(&err);
        return 1;
    }
    if (!ret) {
        fprintf(stderr, "Failed to acquire service name.\n");
        return 1;
    }

    DBusObjectPathVTable vtable = {
        .message_function = dbus_request_handler
    };
    dbus_connection_register_object_path(conn, DBUS_OBJECT_PATH, &vtable, NULL);

    printf("DBus service running.\n");
    while (true) {
        dbus_connection_read_write_dispatch(conn, -1);
    }

    dbus_connection_unref(conn);
    return 0;
}
