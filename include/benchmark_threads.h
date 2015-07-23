//
// Created by Ivan Shynkarenka on 21.07.2015.
//

#ifndef CPPBENCHMARK_BENCHMARK_THREADS_H
#define CPPBENCHMARK_BENCHMARK_THREADS_H

#include <future>

#include "benchmark.h"
#include "context_thread.h"
#include "fixture_threads.h"
#include "settings_threads.h"

namespace CppBenchmark {

class BenchmarkThreads : public Benchmark, public virtual FixtureThreads
{
public:
    explicit BenchmarkThreads(const std::string& name, const SettingsThreads& settings = SettingsThreads())
            : Benchmark(name, settings)
    {}
    BenchmarkThreads(const BenchmarkThreads&) = delete;
    BenchmarkThreads(BenchmarkThreads&&) = delete;
    virtual ~BenchmarkThreads() = default;

    BenchmarkThreads& operator=(const BenchmarkThreads&) = delete;
    BenchmarkThreads& operator=(BenchmarkThreads&&) = delete;

protected:
    virtual void RunThread(ContextThread& context) = 0;

private:
    std::vector<std::future<void>> _futures;

    void Launch(LauncherHandler* handler) override;

    // Hide base benchmark run method
    void Run(Context& context) override {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_THREADS_H
