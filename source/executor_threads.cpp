/*!
    \file executor.cpp
    \brief Dynamic benchmarks executor (thread-safe) class implementation
    \author Ivan Shynkarenka
    \date 02.09.2015
    \copyright MIT License
*/

#include "executor_threads.h"

namespace CppBenchmark {

std::shared_ptr<Phase> ExecutorThreads::StartBenchmark(const std::string& benchmark)
{
    std::lock_guard<std::mutex> lock(_mutex);
    return Executor::StartBenchmark(benchmark);
}

void ExecutorThreads::Report(Reporter& reporter)
{
    std::lock_guard<std::mutex> lock(_mutex);
    UpdateBenchmarkThreads();
    return Executor::Report(reporter);
}

void ExecutorThreads::UpdateBenchmarkThreads()
{
    for (auto it = _benchmarks.begin(); it != _benchmarks.end(); ++it)
        Benchmark::UpdateBenchmarkThreads(**it);
}

} // namespace CppBenchmark
