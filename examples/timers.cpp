//
// Created by Ivan Shynkarenka on 31.07.2015.
//

#if defined(_WIN32) || defined(_WIN64)
#define _WIN32_WINNT 0x0601
#endif

#include "macros.h"

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
    static int64_t resolution = std::numeric_limits<int64_t>::max();

    auto current = std::chrono::high_resolution_clock::now();
    int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current - timestamp).count();
    if ((duration > 0) && (duration < resolution)) {
        timestamp = current;
        resolution = duration;
        context.metrics().SetCustom("Resolution", (int)resolution);
    }
}

BENCHMARK("clock", iterations)
{
    static auto timestamp = clock();
    static int64_t resolution = std::numeric_limits<int64_t>::max();

    auto current = clock();
    int64_t duration = current - timestamp;
    if ((duration > 0) && (duration < resolution)) {
        timestamp = current;
        resolution = duration;
        context.metrics().SetCustom("Resolution", (int)(resolution * 1000 * 1000 * 1000 / CLOCKS_PER_SEC));
    }
}

#ifdef _WIN32
BENCHMARK("GetTickCount", iterations)
{
    static DWORD timestamp = GetTickCount();
    static int64_t resolution = std::numeric_limits<int64_t>::max();

    DWORD current = GetTickCount();
    int64_t duration = current - timestamp;
    if ((duration > 0) && (duration < resolution)) {
        timestamp = current;
        resolution = duration;
        context.metrics().SetCustom("Resolution", (int)(resolution * 1000 * 1000));
    }
}
#endif

#ifdef _WIN32
BENCHMARK("GetTickCount64", iterations)
{
    static ULONGLONG timestamp = GetTickCount64();
    static int64_t resolution = std::numeric_limits<int64_t>::max();

    ULONGLONG current = GetTickCount64();
    int64_t duration = current - timestamp;
    if ((duration > 0) && (duration < resolution)) {
        timestamp = current;
        resolution = duration;
        context.metrics().SetCustom("Resolution", (int)(resolution * 1000 * 1000));
    }
}
#endif

#ifdef _WIN32
BENCHMARK("QueryPerformanceCounter", iterations)
{
    static LARGE_INTEGER timestamp{0};
    static int64_t resolution = std::numeric_limits<int64_t>::max();

    LARGE_INTEGER current;
    QueryPerformanceCounter(&current);
    int64_t duration = current.QuadPart - timestamp.QuadPart;
    if ((duration > 0) && (duration < resolution)) {
        timestamp = current;
        resolution = duration;

        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);

        context.metrics().SetCustom("Resolution", (int)(resolution * 1000 * 1000 * 1000 / frequency.QuadPart));
    }
}
#endif

#ifdef __unix__
BENCHMARK("clock_gettime", iterations)
{
    static struct timespec timestamp{0};
    static int64_t resolution = std::numeric_limits<int64_t>::max();

    struct timespec current;
    clock_gettime(CLOCK_REALTIME, &current);
    int64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_nsec - timestamp.tv_nsec);
    if ((duration > 0) && (duration < resolution)) {
        timestamp = current;
        resolution = duration;
        context.metrics().SetCustom("Resolution", (int)resolution);
    }
}
#endif

#ifdef __unix__
BENCHMARK("gettimeofday", iterations)
{
    static struct timeval timestamp{0};
    static int64_t resolution = std::numeric_limits<int64_t>::max();

    struct timeval current;
    gettimeofday(&current, NULL);
    int64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_usec - timestamp.tv_usec) * 1000;
    if ((duration > 0) && (duration < resolution)) {
        timestamp = current;
        resolution = duration;
        context.metrics().SetCustom("Resolution", (int)resolution);
    }
}
#endif

BENCHMARK_MAIN()
