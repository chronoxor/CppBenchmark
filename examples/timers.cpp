//
// Created by Ivan Shynkarenka on 31.07.2015
//

#include "benchmark/cppbenchmark.h"

#include <chrono>
#include <limits>

#if defined(__APPLE__)
#include <mach/mach_time.h>
#endif
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__CYGWIN__)
#include <time.h>
#include <sys/time.h>
#endif
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#include <windows.h>
#undef max
#undef min
#endif

const auto settings = CppBenchmark::Settings().Operations(10000000).Latency(1, 1000000000, 5, false);

BENCHMARK("high_resolution_clock", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static auto latency_timestamp = std::chrono::high_resolution_clock::now();
    static auto resolution_timestamp = std::chrono::high_resolution_clock::now();
    static uint64_t count = 0;

    // Get the current timestamp
    auto current = std::chrono::high_resolution_clock::now();

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = std::chrono::duration_cast<std::chrono::nanoseconds>(current - latency_timestamp).count();
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = std::chrono::duration_cast<std::chrono::nanoseconds>(current - resolution_timestamp).count();
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}

BENCHMARK("clock", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static clock_t latency_timestamp = clock();
    static clock_t resolution_timestamp = clock();
    static uint64_t count = 0;

    // Get the current timestamp
    clock_t current = clock();

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = (current - latency_timestamp) * 1000000000 / CLOCKS_PER_SEC;
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = (current - resolution_timestamp) * 1000000000 / CLOCKS_PER_SEC;
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}

#if defined(linux) || defined(__linux) || defined(__linux__) || defined(__APPLE__) || defined(__CYGWIN__)

struct timespec clock_gettime(clockid_t clockid)
{
    struct timespec current;
    clock_gettime(clockid, &current);
    return current;
}

BENCHMARK("clock_gettime-CLOCK_REALTIME", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static struct timespec latency_timestamp = clock_gettime(CLOCK_REALTIME);
    static struct timespec resolution_timestamp = clock_gettime(CLOCK_REALTIME);
    static uint64_t count = 0;

    // Get the current timestamp
    struct timespec current = clock_gettime(CLOCK_REALTIME);

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = ((current.tv_sec - latency_timestamp.tv_sec) * 1000000000) + (current.tv_nsec - latency_timestamp.tv_nsec);
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = ((current.tv_sec - resolution_timestamp.tv_sec) * 1000000000) + (current.tv_nsec - resolution_timestamp.tv_nsec);
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}

BENCHMARK("clock_gettime-CLOCK_MONOTONIC", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static struct timespec latency_timestamp = clock_gettime(CLOCK_MONOTONIC);
    static struct timespec resolution_timestamp = clock_gettime(CLOCK_MONOTONIC);
    static uint64_t count = 0;

    // Get the current timestamp
    struct timespec current = clock_gettime(CLOCK_MONOTONIC);

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = ((current.tv_sec - latency_timestamp.tv_sec) * 1000000000) + (current.tv_nsec - latency_timestamp.tv_nsec);
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = ((current.tv_sec - resolution_timestamp.tv_sec) * 1000000000) + (current.tv_nsec - resolution_timestamp.tv_nsec);
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}

#if defined(__APPLE__)

BENCHMARK("clock_gettime_nsec_np-CLOCK_REALTIME", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t latency_timestamp = clock_gettime_nsec_np(CLOCK_REALTIME);
    static uint64_t resolution_timestamp = clock_gettime_nsec_np(CLOCK_REALTIME);
    static uint64_t count = 0;

    // Get the current timestamp
    uint64_t current = clock_gettime_nsec_np(CLOCK_REALTIME);

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = current - latency_timestamp;
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = current - resolution_timestamp;
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}

BENCHMARK("clock_gettime_nsec_np-CLOCK_MONOTONIC", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t latency_timestamp = clock_gettime_nsec_np(CLOCK_MONOTONIC);
    static uint64_t resolution_timestamp = clock_gettime_nsec_np(CLOCK_MONOTONIC);
    static uint64_t count = 0;

    // Get the current timestamp
    uint64_t current = clock_gettime_nsec_np(CLOCK_MONOTONIC);

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = current - latency_timestamp;
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = current - resolution_timestamp;
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}

uint64_t PrepareTimebaseInfo(mach_timebase_info_data_t& tb)
{
    kern_return_t mtiStatus = mach_timebase_info(&tb);
    if (mtiStatus != KERN_SUCCESS)
        return 0;

    return mach_absolute_time();
}

BENCHMARK("mach_absolute_time", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static mach_timebase_info_data_t info;
    static uint64_t bias = PrepareTimebaseInfo(info);
    static uint64_t latency_timestamp = ((mach_absolute_time() - bias) * info.numer) / info.denom;
    static uint64_t resolution_timestamp = ((mach_absolute_time() - bias) * info.numer) / info.denom;
    static uint64_t count = 0;

    // Get the current timestamp
    uint64_t current = ((mach_absolute_time() - bias) * info.numer) / info.denom;

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = current - latency_timestamp;
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = current - resolution_timestamp;
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
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

BENCHMARK("gettimeofday", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static struct timeval latency_timestamp = mygettimeofday();
    static struct timeval resolution_timestamp = mygettimeofday();
    static uint64_t count = 0;

    // Get the current timestamp
    struct timeval current = mygettimeofday();

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = ((current.tv_sec - latency_timestamp.tv_sec) * 1000000000) + (current.tv_usec - latency_timestamp.tv_usec) * 1000;
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = ((current.tv_sec - resolution_timestamp.tv_sec) * 1000000000) + (current.tv_usec - resolution_timestamp.tv_usec) * 1000;
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}

#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

BENCHMARK("GetSystemTimePreciseAsFileTime", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t latency_timestamp = 0;
    static uint64_t resolution_timestamp = 0;
    static uint64_t count = 0;

    FILETIME filetime;
    GetSystemTimePreciseAsFileTime(&filetime);

    ULARGE_INTEGER ularge;
    ularge.LowPart = filetime.dwLowDateTime;
    ularge.HighPart = filetime.dwHighDateTime;
    uint64_t current = ularge.QuadPart * 100;

    if (latency_timestamp == 0)
        latency_timestamp = current;
    if (resolution_timestamp == 0)
        resolution_timestamp = current;

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = current - latency_timestamp;
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = current - resolution_timestamp;
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}

#endif

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)

BENCHMARK("GetTickCount", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static DWORD latency_timestamp = GetTickCount();
    static DWORD resolution_timestamp = GetTickCount();
    static uint64_t count = 0;

    // Get the current timestamp
    DWORD current = GetTickCount();

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = (current - latency_timestamp) * 1000000;
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = (current - resolution_timestamp) * 1000000;
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}

#endif

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__CYGWIN__)

BENCHMARK("GetTickCount64", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static ULONGLONG latency_timestamp = GetTickCount64();
    static ULONGLONG resolution_timestamp = GetTickCount64();
    static uint64_t count = 0;

    // Get the current timestamp
    ULONGLONG current = GetTickCount64();

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = (current - latency_timestamp) * 1000000;
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = (current - resolution_timestamp) * 1000000;
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
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

BENCHMARK("QueryPerformanceCounter", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static LARGE_INTEGER frequency = QueryPerformanceFrequency();
    static LARGE_INTEGER latency_timestamp = QueryPerformanceCounter();
    static LARGE_INTEGER resolution_timestamp = QueryPerformanceCounter();
    static uint64_t count = 0;

    // Get the current timestamp
    LARGE_INTEGER current = QueryPerformanceCounter();

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = (current.QuadPart - latency_timestamp.QuadPart) * 1000000000 / frequency.QuadPart;
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = (current.QuadPart - resolution_timestamp.QuadPart) * 1000000000 / frequency.QuadPart;
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max", maxresolution);
        }
        resolution_timestamp = current;
    }
}

#endif

#if defined(_MSC_VER)

BENCHMARK("RDTSC", settings)
{
    static uint64_t minresolution = std::numeric_limits<uint64_t>::max();
    static uint64_t maxresolution = std::numeric_limits<uint64_t>::min();
    static uint64_t latency_timestamp = __rdtsc();
    static uint64_t resolution_timestamp = __rdtsc();
    static uint64_t count = 0;

    // Get the current timestamp
    uint64_t current = __rdtsc();

    // Update operations counter
    ++count;

    // Register latency metrics
    uint64_t latency = current - latency_timestamp;
    if (latency > 0)
    {
        context.metrics().AddLatency(latency / count);
        latency_timestamp = current;
        count = 0;
    }

    // Register resolution metrics
    uint64_t resolution = current - resolution_timestamp;
    if (resolution > 0)
    {
        if (resolution < minresolution)
        {
            minresolution = resolution;
            context.metrics().SetCustom("resolution-min-clock-cycles", minresolution);
        }
        if (resolution > maxresolution)
        {
            maxresolution = resolution;
            context.metrics().SetCustom("resolution-max-clock-cycles", maxresolution);
        }
        resolution_timestamp = current;
    }
}

#endif

BENCHMARK_MAIN()
