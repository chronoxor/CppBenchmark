//
// Created by Ivan Shynkarenka on 22.07.2015.
//

#ifndef CPPBENCHMARK_LAUNCHER_HANDLER_H
#define CPPBENCHMARK_LAUNCHER_HANDLER_H

#include "benchmark.h"
#include "benchmark_threads.h"

namespace CppBenchmark {

class LauncherHandler
{
    friend class Benchmark;
    friend class BenchmarkMPMC;
    friend class BenchmarkThreads;

public:
    LauncherHandler() = default;
    LauncherHandler(const LauncherHandler&) = default;
    LauncherHandler(LauncherHandler&&) = default;
    virtual ~LauncherHandler() = default;

    LauncherHandler& operator=(const LauncherHandler&) = default;
    LauncherHandler& operator=(LauncherHandler&&) = default;

protected:
    virtual void onLaunching(const Benchmark& benchmark, const Context& context, int attempt) {}
    virtual void onLaunched(const Benchmark& benchmark, const Context& context, int attempt) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_LAUNCHER_HANDLER_H
