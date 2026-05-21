#include "bind_cpu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stddef.h>
#include <assert.h>


static double get_time_ns()
{
    struct timespec time;
    timespec_get(&time, TIME_UTC);
    return (double)time.tv_sec * 1e9 + (double)time.tv_nsec;
}

static double get_time_second()
{
    return get_time_ns() * 1e-9;
}

// 获取CPU频率，粗略测量
static double get_freq_GHz()
{
    const size_t loop = 5e9;
    const size_t begin = rand();
    const size_t end = rand() + loop;
    double t1 = get_time_ns();
    volatile size_t x = rand();
    for ( size_t i = begin; i < end; ++i)
    {
        x += i;
    }
    double t2 = get_time_ns();
    double time = t2 - t1;
    return loop / time;
}

void memcpy_bandwidth_test()
{
    const double FREQ = get_freq_GHz();
    printf("Freq: %.2lf GHz\n", FREQ);
    const size_t MAX_LEN = (size_t)1 << 30;
    const size_t COPY_LEN = (size_t)1 << 35;
    char *src = (char *)malloc(MAX_LEN);
    char *dst = (char *)malloc(MAX_LEN);
    for (size_t len = 128; len <= MAX_LEN; len *= 2)
    {
        size_t loop = COPY_LEN / len;
        memset(dst, rand(), len);
        memset(src, rand(), len);
        double start = get_time_ns();
        for (size_t i = 0; i < loop; ++i)
        {
            memcpy(dst, src, len);
        }
        double end = get_time_ns();
        double time = end - start;
        size_t bytes = len * loop;
        printf("len: %zu Byte\t%.2f GB / s\n", len, bytes / time * 1e9 / 1024 / 1024 / 1024);
        src[rand() % MAX_LEN] = dst[rand() % MAX_LEN];
    }
    free(src);
    free(dst);
}

int main()
{
    int cpu;
    printf("Enter CPU ID: ");
    assert(scanf("%d", &cpu) == 1);
    bind_cpu(cpu);
    memcpy_bandwidth_test();
}