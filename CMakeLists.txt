cmake_minimum_required(VERSION 3.15.6)
project(cndrv_test)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -std=c11 -pthread")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(common)
# add_subdirectory(bw_sample)
add_subdirectory(cnapi)
add_subdirectory(bw)
