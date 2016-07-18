cmake_minimum_required(VERSION 2.8)
project(EEProject)
ENABLE_LANGUAGE(C)
INCLUDE (CheckSymbolExists)
find_package (Threads)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
find_package(WiringPi REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

CHECK_SYMBOL_EXISTS(
        IP6T_SO_ORIGINAL_DST
        "netinet/in.h;net/if.h;linux/netfilter_ipv6/ip6_tables.h"
        HAVE_IP6T_SO_ORIGINAL_DST
)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries

include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${GSTREAMER_INCLUDE_DIRS})
include_directories(${WIRINGPI_INCLUDE_DIRS}) 

link_directories(${GTK3_LIBRARY_DIRS})
link_directories(${GSTREAMER_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})
set(SOURCE_FILES Application/main.c)


add_executable(app Application/main.c Application/connection.h Application/connection.c Application/audio.c Application/audio.h)
add_executable(client Client/main.c)
add_executable(server Server/server.c)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "Application")

# Link the target to the GTK+ libraries
target_link_libraries(app ${GTK3_LIBRARIES})
target_link_libraries(app ${GSTREAMER_LIBRARIES})
target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(client ${WIRINGPI_LIBRARIES}) 


