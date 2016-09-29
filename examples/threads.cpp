//
// Created by Ivan Shynkarenka on 23.07.2015.
//

#include "benchmark/cppbenchmark.h"

#include <atomic>
#include <mutex>

const uint64_t iterations = 10000000;
const auto settings = CppBenchmark::Settings().Iterations(iterations).ThreadsRange(1, 8, [](int from, int to, int& result) { int r = result; result *= 2; return r; });

class UnsynchronizedPreset
{
protected:
    int counter;
};

class AtomicPreset
{
protected:
    std::atomic<int> counter;
};

class MutexPreset
{
protected:
    std::mutex mutex;
    int counter;
};

BENCHMARK_THREADS_PRESET(UnsynchronizedPreset, "unsynchronized++", settings)
{
    ++counter;
}

BENCHMARK_THREADS_PRESET(AtomicPreset, "std::atomic++", settings)
{
    ++counter;
}

BENCHMARK_THREADS_PRESET(MutexPreset, "std::mutex++", settings)
{
    std::lock_guard<std::mutex> lock(mutex);
    ++counter;
}

BENCHMARK_MAIN()
