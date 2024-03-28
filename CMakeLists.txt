cmake_minimum_required(VERSION 3.15.6)
project(cndrv_test)

include_directories($ENV{NEUWARE_HOME}/include)
link_directories($ENV{NEUWARE_HOME}/lib64)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=c11")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(test test.c)
target_link_libraries(test cndrv)