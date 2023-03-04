cmake_minimum_required(VERSION 3.5)
project(dbus_example C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)
pkg_check_modules(GLIB REQUIRED glib-2.0)

include_directories(${DBUS_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS})

# Build daemon executable
add_executable(shellrunner deamon.c)
target_link_libraries(shellrunner ${DBUS_LIBRARIES} ${GLIB_LIBRARIES})

# Build test program
add_executable(
    client_test
    shell_runner.c
    client_test.c
)
target_link_libraries(client_test ${DBUS_LIBRARIES})

# Install daemon executable to system binary directory
install(TARGETS shellrunner DESTINATION /usr/bin)
