#ifndef COMMON_H
#define COMMON_H

#include <time.h>

void time_start();
void time_end();

#define TIME_CALC(func) do { \
    time_start();                  \
    func;                    \
    time_end();                    \
} while(0)

#endif // COMMON_H