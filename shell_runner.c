#include "shell_runner.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dbus/dbus.h>

#define DBUS_DAEMON_NAME        SHELL_RUNNER_DBUS_SERVICE_NAME
#define DBUS_DAEMON_PATH        SHELL_RUNNER_DBUS_OBJECT_PATH
#define DBUS_DAEMON_INTERFACE   SHELL_RUNNER_DBUS_INTERFACE_NAME
#define DBUS_DAEMON_METHOD      "ExecuteCommand"

int shell_runner_exec(char* command, char** result, int *exitcode)
{
    DBusError error;
    DBusConnection *connection;
    DBusMessage *request, *reply;
    DBusMessageIter iter;
    int ret = -1;

    dbus_error_init(&error);

    // Connect to the bus
    connection = dbus_bus_get(DBUS_BUS_SESSION, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Connection Error (%s)\n", error.message);
        dbus_error_free(&error);
        return ret;
    }

    // Create a new method call and check for errors
    request = dbus_message_new_method_call(DBUS_DAEMON_NAME,
                                           DBUS_DAEMON_PATH,
                                           DBUS_DAEMON_INTERFACE,
                                           DBUS_DAEMON_METHOD);
    if (request == NULL) {
        fprintf(stderr, "Message Null\n");
        return ret;
    }

    // Append arguments to the method call
    dbus_message_iter_init_append(request, &iter);
    if (!dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &command)) {
        fprintf(stderr, "Out Of Memory!\n");
        return ret;
    }

    // Send the message and get a reply
    reply = dbus_connection_send_with_reply_and_block(connection, request, -1, &error);
    if (dbus_error_is_set(&error)) {
        fprintf(stderr, "Reply Error (%s)\n", error.message);
        dbus_error_free(&error);
        return ret;
    }

    // Parse the reply and get the result
    DBusBasicValue dbus_result, dbus_exitcode;
    dbus_message_iter_init(reply, &iter);
    dbus_message_iter_get_basic(&iter, &dbus_result);
    *result = malloc(strlen(dbus_result.str) + 1);
    strcpy(*result, dbus_result.str);

    dbus_message_iter_next(&iter);
    dbus_message_iter_get_basic(&iter, &dbus_exitcode);
    *exitcode = dbus_exitcode.i32;

    // Free the reply and close the connection
    dbus_message_unref(reply);
    dbus_connection_close(connection);
    return 0;
}
