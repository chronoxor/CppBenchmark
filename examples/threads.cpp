//
// Created by Ivan Shynkarenka on 23.07.2015.
//

#include "macros.h"

#include <atomic>
#include <mutex>

const int iterations = 10000000;
const auto settings = CppBenchmark::Settings().Iterations(iterations).ThreadsRange(1, 32, [](int from, int to, int result) { return (result * 2); });

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

BENCHMARK_THREADS_FIXTURE(UnsynchronizedFixture, "unsynchronized++", settings)
{
    counter++;
}

BENCHMARK_THREADS_FIXTURE(AtomicFixture, "std::atomic++", settings)
{
    counter++;
}

BENCHMARK_THREADS_FIXTURE(MutexFixture, "std::mutex++", settings)
{
    std::lock_guard<std::mutex> lock(mutex);
    counter++;
}

BENCHMARK_MAIN()
