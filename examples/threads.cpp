//
// Created by Ivan Shynkarenka on 23.07.2015.
//

#include "macros.h"

#include <atomic>
#include <mutex>

class UnsynchronizedFixture
{
protected:
    int counter;
};

class AtomicFixture
{
protected:
    std::atomic<int> counter;
};

class MutexFixture
{
protected:
    std::mutex mutex;
    int counter;
};

BENCHMARK_THREADS_FIXTURE(UnsynchronizedFixture, "unsynchronized++", Settings().ThreadsRange(1, 32, [](int, int, int result) {return (result * 2); }).Param(10000000))
{
    for (int i = 0; i < context.x(); ++i) {
        counter++;
        context.metrics().AddItems(1);
    }
}

BENCHMARK_THREADS_FIXTURE(AtomicFixture, "std::atomic++", Settings().ThreadsRange(1, 32, [](int, int, int result) {return (result * 2); }).Param(10000000))
{
    for (int i = 0; i < context.x(); ++i) {
        counter++;
        context.metrics().AddItems(1);
    }
}

BENCHMARK_THREADS_FIXTURE(MutexFixture, "std::mutex++", Settings().ThreadsRange(1, 32, [](int, int, int result) {return (result * 2); }).Param(100000))
{
    for (int i = 0; i < context.x(); ++i) {
        std::lock_guard<std::mutex> lock(mutex);
        counter++;
        context.metrics().AddItems(1);
    }
}

BENCHMARK_MAIN()
