#ifndef COMMON_H
#define COMMON_H

#include <time.h>

void time_start();
double time_end_sec();
double time_end_msec();
long time_end_usec();
long long time_end_nsec();

#define TIME_CALC(func)  \
    do {                 \
        time_start();    \
        func;            \
        time_end_nsec(); \
    } while (0)

#endif  // COMMON_H