# DBusShellRunner

> 注意：本项目使用ChatGPT编写，然后人工debug修改程序完成，不要在严肃场景下使用！

已知问题：

- 第二次执行命令，deamon会崩溃
- shell_runner客户端，似乎没处理好DBus通讯异常时的情况：

```
Reply Error (Message recipient disconnected from message bus without replying)
DBus Command failed, return -1
munmap_chunk(): invalid pointer
```


DBus命令执行服务是一个基于DBus协议的守护进程，提供了一个DBus服务，可以让其他程序通过DBus发送命令并执行，然后返回执行结果。它通过DBus实现了程序间的通信，提供了一种安全、高效、可靠的方式来执行命令并返回结果。

该服务可以通过DBus接口暴露给其他程序，支持执行任意Shell命令，并返回命令执行结果及退出码。它还提供了一些简单的安全控制机制，确保只有授权的程序可以发送命令并执行。

通过DBus命令执行服务，其他程序可以方便地执行Shell命令，从而实现更灵活、更高效的系统管理和自动化操作。该服务可以在Linux等系统上运行，并支持多种编程语言。


DBus Command Execution Service is a daemon based on the DBus protocol, which provides a DBus service that allows other programs to send commands via DBus and execute them, and then return the execution results. It uses DBus to implement interprocess communication, providing a secure, efficient, and reliable way to execute commands and return results.

The service can be exposed to other programs via the DBus interface, supporting the execution of any shell commands and returning command execution results and exit codes. It also provides some simple security control mechanisms to ensure that only authorized programs can send commands and execute them.

With the DBus Command Execution Service, other programs can easily execute shell commands, thereby achieving more flexible and efficient system management and automation operations. The service can run on Linux and other systems and supports multiple programming languages.

## 测试命令

```
$ dbus-send --session --dest=com.example.MyService --type=method_call --print-reply /com/example/MyService com.example.MyInterface.ExecuteShellCommand string:"ls -l"
```


