//
// Created by Ivan Shynkarenka on 31.07.2015.
//

#if defined(__CYGWIN__) || (__MINGW32__)
#define _WIN32_WINNT 0x601
#endif

#include "benchmark/cppbenchmark.h"

#include <chrono>
#include <limits>

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__CYGWIN__)
#include <sys/time.h>
#include <time.h>
#endif
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#undef max
#undef min
#endif

const uint64_t iterations = 10000000;

BENCHMARK("std::chrono::high_resolution_clock::now()", iterations)
{
    static auto timestamp = std::chrono::high_resolution_clock::now();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    auto current = std::chrono::high_resolution_clock::now();
    uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current - timestamp).count();
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

BENCHMARK("clock()", iterations)
{
    static auto timestamp = clock();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    auto current = clock();
    uint64_t duration = (current - timestamp) * 1000 * 1000 * 1000 / CLOCKS_PER_SEC;
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

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__CYGWIN__)
struct timespec clock_gettime(clockid_t clockid)
{
    struct timespec current;
    clock_gettime(clockid, &current);
    return current;
}

BENCHMARK("clock_gettime(CLOCK_REALTIME)", iterations)
{
    static struct timespec timestamp = clock_gettime(CLOCK_REALTIME);
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    struct timespec current = clock_gettime(CLOCK_REALTIME);
    uint64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_nsec - timestamp.tv_nsec);
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

BENCHMARK("clock_gettime(CLOCK_MONOTONIC)", iterations)
{
    static struct timespec timestamp = clock_gettime(CLOCK_MONOTONIC);
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    struct timespec current = clock_gettime(CLOCK_MONOTONIC);
    uint64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_nsec - timestamp.tv_nsec);
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

#if !defined(__CYGWIN__)
BENCHMARK("clock_gettime(CLOCK_MONOTONIC_COARSE)", iterations)
{
    static struct timespec timestamp = clock_gettime(CLOCK_MONOTONIC_COARSE);
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    struct timespec current = clock_gettime(CLOCK_MONOTONIC_COARSE);
    uint64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_nsec - timestamp.tv_nsec);
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

BENCHMARK("clock_gettime(CLOCK_MONOTONIC_RAW)", iterations)
{
    static struct timespec timestamp = clock_gettime(CLOCK_MONOTONIC_RAW);
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    struct timespec current = clock_gettime(CLOCK_MONOTONIC_RAW);
    uint64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_nsec - timestamp.tv_nsec);
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

BENCHMARK("clock_gettime(CLOCK_REALTIME_COARSE)", iterations)
{
    static struct timespec timestamp = clock_gettime(CLOCK_REALTIME_COARSE);
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    struct timespec current = clock_gettime(CLOCK_REALTIME_COARSE);
    uint64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_nsec - timestamp.tv_nsec);
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
#endif

#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
struct timeval mygettimeofday()
{
    struct timeval current;
    gettimeofday(&current, nullptr);
    return current;
}

BENCHMARK("gettimeofday()", iterations)
{
    static struct timeval timestamp = mygettimeofday();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    struct timeval current = mygettimeofday();
    uint64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_usec - timestamp.tv_usec) * 1000;
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

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
BENCHMARK("GetSystemTimePreciseAsFileTime()", iterations)
{
    static uint64_t timestamp = 0;
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    FILETIME filetime;
    GetSystemTimePreciseAsFileTime(&filetime);

    ULARGE_INTEGER ularge;
    ularge.LowPart = filetime.dwLowDateTime;
    ularge.HighPart = filetime.dwHighDateTime;
    uint64_t current = ularge.QuadPart * 100;

    if (timestamp == 0)
        timestamp = current;

    uint64_t duration = (current - timestamp);
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

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
BENCHMARK("GetTickCount()", iterations)
{
    static DWORD timestamp = GetTickCount();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    DWORD current = GetTickCount();
    uint64_t duration = (current - timestamp) * 1000 * 1000;
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

#if defined(_WIN32) || defined(_WIN64)
BENCHMARK("GetTickCount64()", iterations)
{
    static ULONGLONG timestamp = GetTickCount64();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    ULONGLONG current = GetTickCount64();
    uint64_t duration = (current - timestamp) * 1000 * 1000;
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

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
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

BENCHMARK("QueryPerformanceCounter()", iterations)
{
    static LARGE_INTEGER frequency = QueryPerformanceFrequency();
    static LARGE_INTEGER timestamp = QueryPerformanceCounter();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    LARGE_INTEGER current = QueryPerformanceCounter();
    uint64_t duration = (current.QuadPart - timestamp.QuadPart) * 1000 * 1000 * 1000 / frequency.QuadPart;
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

#if defined(_MSC_VER)
BENCHMARK("__rdtsc()", iterations)
{
    static uint64_t timestamp = __rdtsc();
    static double maxlatency = std::numeric_limits<double>::min();
    static double minlatency = std::numeric_limits<double>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t count = 0;

    uint64_t current = __rdtsc();
    uint64_t duration = current - timestamp;
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

BENCHMARK_MAIN()
