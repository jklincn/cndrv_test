#include "common.h"

#include <stdio.h>
#include <time.h>

static struct timespec start_time;

// 开始计时
void time_start() { timespec_get(&start_time, TIME_UTC); }

// 结束计时并打印经过的时间
void time_end() {
    struct timespec end_time;
    timespec_get(&end_time, TIME_UTC);
    // 现在将时间差转换为微秒
    long elapsed = (end_time.tv_sec - start_time.tv_sec) * 1000000L +
                   (end_time.tv_nsec - start_time.tv_nsec) / 1000L;
    printf("Time elapsed: %ld microseconds\n", elapsed);
}
