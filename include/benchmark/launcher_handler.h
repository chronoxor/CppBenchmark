/*!
    \file launcher_handler.h
    \brief Launcher handler definition
    \author Ivan Shynkarenka
    \date 22.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_LAUNCHER_HANDLER_H
#define CPPBENCHMARK_LAUNCHER_HANDLER_H

#include "benchmark/benchmark_base.h"
#include "benchmark/benchmark_threads.h"

namespace CppBenchmark {

//! Launcher handler
/*!
    Provides interface to handle progress notifications from benchmark launcher.
*/
class LauncherHandler
{
    friend class Benchmark;
    friend class BenchmarkPC;
    friend class BenchmarkThreads;

public:
    LauncherHandler() = default;
    LauncherHandler(const LauncherHandler&) = default;
    LauncherHandler(LauncherHandler&&) noexcept = default;
    virtual ~LauncherHandler() = default;

    LauncherHandler& operator=(const LauncherHandler&) = default;
    LauncherHandler& operator=(LauncherHandler&&) noexcept = default;

protected:
    //! Handle benchmark launching notification
    /*!
        This method is called before launching the given benchmark.

        \param current - Current benchmark number
        \param total - Total benchmarks
        \param benchmark - Benchmark
        \param context - Benchmark running context
        \param attempt - Benchmark attempt
    */
    virtual void onLaunching(int current, int total, const BenchmarkBase& benchmark, const Context& context, int attempt) {}
    //! Handle benchmark launched notification
    /*!
        This method is called after launched the given benchmark.

        \param current - Current benchmark number
        \param total - Total benchmarks
        \param benchmark - Benchmark
        \param context - Benchmark running context
        \param attempt - Benchmark attempt
    */
    virtual void onLaunched(int current, int total, const BenchmarkBase& benchmark, const Context& context, int attempt) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_LAUNCHER_HANDLER_H
