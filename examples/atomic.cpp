//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#include "benchmark/cppbenchmark.h"

#include <atomic>

const uint64_t iterations = 100000000;

template <typename T>
class AtomicPreset
{
protected:
    std::atomic<T> x;
    T y;
    T z;
};

BENCHMARK_PRESET(AtomicPreset<int32_t>, "std::atomic<int32_t>.CAS()", iterations)
{
    x.compare_exchange_strong(y, z, std::memory_order_acq_rel);
}

BENCHMARK_PRESET(AtomicPreset<int64_t>, "std::atomic<int64_t>.CAS()", iterations)
{
    x.compare_exchange_strong(y, z, std::memory_order_acq_rel);
}

BENCHMARK_PRESET(AtomicPreset<int32_t>, "std::atomic<int32_t>.FAA()", iterations)
{
    x.fetch_add(y, std::memory_order_acq_rel);
}

BENCHMARK_PRESET(AtomicPreset<int64_t>, "std::atomic<int64_t>.FAA()", iterations)
{
    x.fetch_add(y, std::memory_order_acq_rel);
}

BENCHMARK_MAIN()
