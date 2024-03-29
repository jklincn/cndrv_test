add_executable(test test.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

include_directories($ENV{NEUWARE_HOME}/include)

find_library(CNDRV_LIBRARY NAMES cndrv PATHS $ENV{NEUWARE_HOME}/lib64 NO_DEFAULT_PATH)
if(NOT CNDRV_LIBRARY)
    message(FATAL_ERROR "cndrv library not found")
endif()

target_link_libraries(test ${CNDRV_LIBRARY})
target_link_libraries(test common_lib)