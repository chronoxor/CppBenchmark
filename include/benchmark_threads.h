//
// Created by Ivan Shynkarenka on 21.07.2015.
//

#ifndef CPPBENCHMARK_BENCHMARK_THREADS_H
#define CPPBENCHMARK_BENCHMARK_THREADS_H

#include <thread>

#include "benchmark.h"
#include "context_thread.h"
#include "fixture_threads.h"
#include "settings_threads.h"

namespace CppBenchmark {

class BenchmarkThreads : public Benchmark, public virtual FixtureThreads
{
    friend class BenchmarkMPMC;

public:
    typedef SettingsThreads TSettings;			

    explicit BenchmarkThreads(const std::string& name, const TSettings& settings = TSettings()) : Benchmark(name, settings) {}
    BenchmarkThreads(const BenchmarkThreads&) = delete;
    BenchmarkThreads(BenchmarkThreads&&) = delete;
    virtual ~BenchmarkThreads() = default;

    BenchmarkThreads& operator=(const BenchmarkThreads&) = delete;
    BenchmarkThreads& operator=(BenchmarkThreads&&) = delete;

protected:
    virtual void RunThread(ContextThread& context) = 0;

private:
    std::vector<std::thread> _threads;

    void Launch(LauncherHandler* handler) override;

    // Hide base benchmark run method
    void Run(Context& context) override {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_THREADS_H
