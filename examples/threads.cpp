//
// Created by Ivan Shynkarenka on 23.07.2015
//

#include "benchmark/cppbenchmark.h"

#include <atomic>
#include <mutex>

const auto settings = CppBenchmark::Settings().ThreadsRange(1, 8, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

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
    ++counter;
}

BENCHMARK_THREADS_FIXTURE(AtomicFixture, "std::atomic++", settings)
{
    ++counter;
}

BENCHMARK_THREADS_FIXTURE(MutexFixture, "std::mutex++", settings)
{
    std::scoped_lock lock(mutex);
    ++counter;
}

BENCHMARK_MAIN()
