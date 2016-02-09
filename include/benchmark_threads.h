/*!
    \file benchmark_threads.h
    \brief Threads benchmark base definition
    \author Ivan Shynkarenka
    \date 21.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_BENCHMARK_THREADS_H
#define CPPBENCHMARK_BENCHMARK_THREADS_H

#include "benchmark.h"
#include "context_thread.h"
#include "fixture_threads.h"
#include "settings_threads.h"

#include <thread>

namespace CppBenchmark {

//! Threads benchmark base class
/*!
    Provides interface to perform benchmark in multi-thread environment.
*/
class BenchmarkThreads : public Benchmark, public virtual FixtureThreads
{
    friend class BenchmarkPC;

public:
    //! BenchmarkThreads settings type
    typedef SettingsThreads TSettings;

    //! Default class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    explicit BenchmarkThreads(const std::string& name, const TSettings& settings = TSettings()) : Benchmark(name, settings) {}
    BenchmarkThreads(const BenchmarkThreads&) = delete;
    BenchmarkThreads(BenchmarkThreads&&) = delete;
    virtual ~BenchmarkThreads() = default;

    BenchmarkThreads& operator=(const BenchmarkThreads&) = delete;
    BenchmarkThreads& operator=(BenchmarkThreads&&) = delete;

protected:
    //! Thread run method
    /*!
        Should be implemented to provide code which will be executed in separate thread!

        \param context - Thread running context
    */
    virtual void RunThread(ContextThread& context) = 0;

private:
    std::vector<std::thread> _threads;

    int CountLaunches() const override;
    void Launch(int& current, int total, LauncherHandler& handler) override;

    // Hide base benchmark run method
    void Run(Context& context) override {}
};

/*! \example threads.cpp Threads integer increment benchmark */

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_THREADS_H
