//
// Created by Ivan Shynkarenka on 31.07.2015.
//

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0601
#endif

#include "cppbenchmark.h"

#include <chrono>
#include <ctime>
#include <limits>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#undef max
#undef min
#elif defined(unix) || defined(__unix) || defined(__unix__)
#include <sys/time.h>
#include <time.h>
#endif

const int iterations = 10000000;

BENCHMARK("std::chrono::high_resolution_clock::now", iterations)
{
    static auto timestamp = std::chrono::high_resolution_clock::now();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static int64_t maxresolution = std::numeric_limits<int64_t>::min();
    static int64_t minresolution = std::numeric_limits<int64_t>::max();
    static int64_t count = 0;

    auto current = std::chrono::high_resolution_clock::now();
    int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current - timestamp).count();
    double latency = (double)duration / ++count;
    if (duration > 0)
    {
        if (latency < minlatency)
        {
            minlatency = latency;
            context.metrics().SetCustom("latency-min", minlatency);
        }
        if (latency > maxlatency)
        {
            maxlatency = latency;
            context.metrics().SetCustom("latency-max", maxlatency);
        }
        if (duration < minresolution)
        {
            minresolution = duration;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (duration > maxresolution)
        {
            maxresolution = duration;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        timestamp = current;
        count = 0;
    }
}

BENCHMARK("clock", iterations)
{
    static auto timestamp = clock();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static int64_t maxresolution = std::numeric_limits<int64_t>::min();
    static int64_t minresolution = std::numeric_limits<int64_t>::max();
    static int64_t count = 0;

    auto current = clock();
    int64_t duration = (current - timestamp) * 1000 * 1000 * 1000 / CLOCKS_PER_SEC;
    double latency = (double)duration / ++count;
    if (duration > 0)
    {
        if (latency < minlatency)
        {
            minlatency = latency;
            context.metrics().SetCustom("latency-min", minlatency);
        }
        if (latency > maxlatency)
        {
            maxlatency = latency;
            context.metrics().SetCustom("latency-max", maxlatency);
        }
        if (duration < minresolution)
        {
            minresolution = duration;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (duration > maxresolution)
        {
            maxresolution = duration;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        timestamp = current;
        count = 0;
    }
}

#ifdef _WIN32
BENCHMARK("GetTickCount", iterations)
{
    static DWORD timestamp = GetTickCount();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static int64_t maxresolution = std::numeric_limits<int64_t>::min();
    static int64_t minresolution = std::numeric_limits<int64_t>::max();
    static int64_t count = 0;

    DWORD current = GetTickCount();
    int64_t duration = (current - timestamp) * 1000 * 1000;
    double latency = (double)duration / ++count;
    if (duration > 0)
    {
        if (latency < minlatency)
        {
            minlatency = latency;
            context.metrics().SetCustom("latency-min", minlatency);
        }
        if (latency > maxlatency)
        {
            maxlatency = latency;
            context.metrics().SetCustom("latency-max", maxlatency);
        }
        if (duration < minresolution)
        {
            minresolution = duration;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (duration > maxresolution)
        {
            maxresolution = duration;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        timestamp = current;
        count = 0;
    }
}
#endif

#ifdef _WIN32
BENCHMARK("GetTickCount64", iterations)
{
    static ULONGLONG timestamp = GetTickCount64();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static int64_t maxresolution = std::numeric_limits<int64_t>::min();
    static int64_t minresolution = std::numeric_limits<int64_t>::max();
    static int64_t count = 0;

    ULONGLONG current = GetTickCount64();
    int64_t duration = (current - timestamp) * 1000 * 1000;
    double latency = (double)duration / ++count;
    if (duration > 0)
    {
        if (latency < minlatency)
        {
            minlatency = latency;
            context.metrics().SetCustom("latency-min", minlatency);
        }
        if (latency > maxlatency)
        {
            maxlatency = latency;
            context.metrics().SetCustom("latency-max", maxlatency);
        }
        if (duration < minresolution)
        {
            minresolution = duration;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (duration > maxresolution)
        {
            maxresolution = duration;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        timestamp = current;
        count = 0;
    }
}
#endif

#ifdef _WIN32
LARGE_INTEGER QueryPerformanceCounter()
{
    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);
    return current;
}

LARGE_INTEGER QueryPerformanceFrequency()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency;
}

BENCHMARK("QueryPerformanceCounter", iterations)
{
    static LARGE_INTEGER frequency = QueryPerformanceFrequency();
    static LARGE_INTEGER timestamp = QueryPerformanceCounter();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static int64_t maxresolution = std::numeric_limits<int64_t>::min();
    static int64_t minresolution = std::numeric_limits<int64_t>::max();
    static int64_t count = 0;

    LARGE_INTEGER current = QueryPerformanceCounter();
    int64_t duration = (current.QuadPart - timestamp.QuadPart) * 1000 * 1000 * 1000 / frequency.QuadPart;
    double latency = (double)duration / ++count;
    if (duration > 0)
    {
        if (latency < minlatency)
        {
            minlatency = latency;
            context.metrics().SetCustom("latency-min", minlatency);
        }
        if (latency > maxlatency)
        {
            maxlatency = latency;
            context.metrics().SetCustom("latency-max", maxlatency);
        }
        if (duration < minresolution)
        {
            minresolution = duration;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (duration > maxresolution)
        {
            maxresolution = duration;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        timestamp = current;
        count = 0;
    }
}
#endif

#ifdef _MSC_VER
BENCHMARK("__rdtsc", iterations)
{
    static int64_t timestamp = __rdtsc();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static int64_t maxresolution = std::numeric_limits<int64_t>::min();
    static int64_t minresolution = std::numeric_limits<int64_t>::max();
    static int64_t count = 0;

    int64_t current = __rdtsc();
    int64_t duration = current - timestamp;
    double latency = (double)duration / ++count;
    if (duration > 0)
    {
        if (latency < minlatency)
        {
            minlatency = latency;
            context.metrics().SetCustom("latency-min-clock-cycles", minlatency);
        }
        if (latency > maxlatency)
        {
            maxlatency = latency;
            context.metrics().SetCustom("latency-max-clock-cycles", maxlatency);
        }
        if (duration < minresolution)
        {
            minresolution = duration;
            context.metrics().SetCustom("resolution-min-clock-cycles", minresolution);
        }
        if (duration > maxresolution)
        {
            maxresolution = duration;
            context.metrics().SetCustom("resolution-max-clock-cycles", maxresolution);
        }
        timestamp = current;
        count = 0;
    }
}
#endif

#ifdef __unix__
struct timespec clock_gettime()
{
    struct timespec current;
    clock_gettime(CLOCK_REALTIME, &current);
    return current;
}

BENCHMARK("clock_gettime", iterations)
{
    static struct timespec timestamp = clock_gettime();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static int64_t maxresolution = std::numeric_limits<int64_t>::min();
    static int64_t minresolution = std::numeric_limits<int64_t>::max();
    static int64_t count = 0;

    struct timespec current = clock_gettime();
    int64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_nsec - timestamp.tv_nsec);
    double latency = (double)duration / ++count;
    if (duration > 0)
    {
        if (latency < minlatency)
        {
            minlatency = latency;
            context.metrics().SetCustom("latency-min", minlatency);
        }
        if (latency > maxlatency)
        {
            maxlatency = latency;
            context.metrics().SetCustom("latency-max", maxlatency);
        }
        if (duration < minresolution)
        {
            minresolution = duration;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (duration > maxresolution)
        {
            maxresolution = duration;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        timestamp = current;
        count = 0;
    }
}
#endif

#ifdef __unix__
struct timeval gettimeofday()
{
    struct timeval current;
    gettimeofday(&current, NULL);
    return current;
}

BENCHMARK("gettimeofday", iterations)
{
    static struct timeval timestamp = gettimeofday();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static int64_t maxresolution = std::numeric_limits<int64_t>::min();
    static int64_t minresolution = std::numeric_limits<int64_t>::max();
    static int64_t count = 0;

    struct timeval current = gettimeofday();
    int64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_usec - timestamp.tv_usec) * 1000;
    double latency = (double)duration / ++count;
    if (duration > 0)
    {
        if (latency < minlatency)
        {
            minlatency = latency;
            context.metrics().SetCustom("latency-min", minlatency);
        }
        if (latency > maxlatency)
        {
            maxlatency = latency;
            context.metrics().SetCustom("latency-max", maxlatency);
        }
        if (duration < minresolution)
        {
            minresolution = duration;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (duration > maxresolution)
        {
            maxresolution = duration;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        timestamp = current;
        count = 0;
    }
}
#endif

BENCHMARK_MAIN()
