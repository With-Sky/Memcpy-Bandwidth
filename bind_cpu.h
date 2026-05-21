#ifndef BIND_CPU_H
#define BIND_CPU_H

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#define _GNU_SOURCE
#include <pthread.h>
#include <sched.h>
#endif
#include <stdio.h>

static void bind_cpu(int cpu_id)
{
#if defined(_WIN32)
    DWORD_PTR handle = SetThreadAffinityMask(GetCurrentThread(), 1ull << cpu_id);
    if (handle == 0)
    {
        printf("Windows bind cpu failed\n");
    }
    else
    {
        printf("Windows bind to cpu %d\n", cpu_id);
    }
#elif defined(__linux__)
    pthread_t tid = pthread_self();
    cpu_set_t cpu_mask;
    CPU_ZERO(&cpu_mask);
    CPU_SET(cpu_id, &cpu_mask);
    int ret = pthread_setaffinity_np(tid, sizeof(cpu_set_t), &cpu_mask);
    if (ret != 0)
    {
        printf("Linux bind cpu failed\n");
    }
    cpu_set_t cpu_get;
    pthread_getaffinity_np(tid, sizeof(cpu_set_t), &cpu_get);
    if (CPU_ISSET(cpu_id, &cpu_get))
    {
        printf("Linux bind to cpu %d\n", cpu_id);
    }
#else
    printf("bind cpu not supported\n");
#endif
}

#endif