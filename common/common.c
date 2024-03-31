#include "common.h"

#include <stdio.h>
#include <time.h>

static struct timespec start_time;

// 开始计时
void time_start() { timespec_get(&start_time, TIME_UTC); }

// 结束计时并返回经过的秒数
double time_end_sec() {
    struct timespec end_time;
    timespec_get(&end_time, TIME_UTC);
    double elapsed = (end_time.tv_sec - start_time.tv_sec) +
                     (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    printf("Time elapsed: %.2f seconds\n", elapsed);
    return elapsed;
}

// 结束计时并返回经过的毫秒数
double time_end_msec() {
    struct timespec end_time;
    timespec_get(&end_time, TIME_UTC);
    double elapsed = (end_time.tv_sec - start_time.tv_sec) * 1e3 +
                     (end_time.tv_nsec - start_time.tv_nsec) / 1e6;
    printf("Time elapsed: %.2f milliseconds\n", elapsed);
    return elapsed;
}

// 结束计时并返回经过的微秒数
long time_end_usec() {
    struct timespec end_time;
    timespec_get(&end_time, TIME_UTC);
    long elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000000L +
                   (end_time.tv_nsec - start_time.tv_nsec) / 1000L;
    printf("Time elapsed: %ld microseconds\n", elapsed);
    return elapsed;
}

// 结束计时并返回经过的纳秒数
long long time_end_nsec() {
    struct timespec end_time;
    timespec_get(&end_time, TIME_UTC);
    long long elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000000000LL +
                        (end_time.tv_nsec - start_time.tv_nsec);
    printf("Time elapsed: %lld nanoseconds\n", elapsed);
    return elapsed;
}
