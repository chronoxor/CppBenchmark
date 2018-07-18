//
// Created by Ivan Shynkarenka on 11.10.2016
//

#include "benchmark/cppbenchmark.h"

#include <chrono>
#include <thread>

const auto settings = CppBenchmark::Settings().Latency(1, 1000000000, 5);

BENCHMARK("sleep", settings)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

BENCHMARK_MAIN()
