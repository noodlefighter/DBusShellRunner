cmake_minimum_required(VERSION 3.16)
project(shellrunner)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(DBUS REQUIRED dbus-1)
pkg_check_modules(GLIB REQUIRED glib-2.0)

include_directories(${DBUS_INCLUDE_DIRS} ${GLIB_INCLUDE_DIRS})

# Build daemon executable
add_executable(shellrunner deamon.c)
target_link_libraries(shellrunner ${DBUS_LIBRARIES} ${GLIB_LIBRARIES})

# Build library
add_library(libshellrunner shell_runner.c)
add_library(shellrunner::libshellrunner ALIAS libshellrunner)
target_link_libraries(libshellrunner PUBLIC ${DBUS_LIBRARIES})

# Build test program
add_executable(
    client_test
    shell_runner.c
    client_test.c
)
target_link_libraries(client_test shellrunner::libshellrunner)

add_executable(
    client_test_cpp
    shell_runner.c
    client_test.cpp
)
target_link_libraries(client_test_cpp shellrunner::libshellrunner)

# Install daemon executable to system binary directory
install(TARGETS shellrunner DESTINATION /usr/bin)
