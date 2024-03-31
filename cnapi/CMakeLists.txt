add_executable(cnapi_test test.c)

include_directories($ENV{NEUWARE_HOME}/include)

find_library(CNDRV_LIBRARY NAMES cndrv PATHS $ENV{NEUWARE_HOME}/lib64 NO_DEFAULT_PATH)
if(NOT CNDRV_LIBRARY)
    message(FATAL_ERROR "cndrv library not found")
endif()

target_link_libraries(cnapi_test ${CNDRV_LIBRARY})
target_link_libraries(cnapi_test common_lib)