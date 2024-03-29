set(SUBDIR_SOURCES bw_sample.c p2p_sample.c)

include_directories($ENV{DRIVER_HOME}/include)
include_directories($ENV{DRIVER_HOME}/core)

add_executable(bw_sample ${SUBDIR_SOURCES})