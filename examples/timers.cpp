//
// Created by Ivan Shynkarenka on 31.07.2015.
//

#ifdef _WIN32
#define _WIN32_WINNT 0x0601
#include <windows.h>
#undef max
#undef min
#endif

#ifdef __unix__
#include <time.h>
#endif

#include "macros.h"

#include <limits>

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
    static timespec timestamp{0};
    static int64_t resolution = std::numeric_limits<int64_t>::max();

    timespec current;
    clock_gettime(CLOCK_REALTIME, &current);
    int64_t duration = ((current.tv_sec - timestamp.tv_sec) * 1000 * 1000 * 1000) + (current.tv_nsec - timestamp.tv_nsec);
    if ((duration > 0) && (duration < resolution)) {
        timestamp = current;
        resolution = duration;
        context.metrics().SetCustom("Resolution", (int)resolution);

        timespec res;
        clock_getres(CLOCK_REALTIME, &res);
        context.metrics().SetCustom("Resolution (system)", (int)((res.tv_sec * 1000 * 1000 * 1000) + res.tv_nsec));
    }
}
#endif

BENCHMARK_MAIN()
