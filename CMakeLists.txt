cmake_minimum_required(VERSION 3.0.2)
project(EEProject)
ENABLE_LANGUAGE(C)
INCLUDE (CheckSymbolExists)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
CHECK_SYMBOL_EXISTS(
        IP6T_SO_ORIGINAL_DST
        "netinet/in.h;net/if.h;linux/netfilter_ipv6/ip6_tables.h"
        HAVE_IP6T_SO_ORIGINAL_DST
)
# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})
set(SOURCE_FILES Application/main.c)
#add_library(MyLib )
add_executable(app Application/main.c Application/connection.h Application/connection.c)
add_executable(client Client/client.c)
add_executable(server Server/server.c)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "Application")
# Link the target to the GTK+ libraries
target_link_libraries(app ${GTK3_LIBRARIES})
#target_link_libraries(app MyLib)
#target_link_libraries(client MyLib)
