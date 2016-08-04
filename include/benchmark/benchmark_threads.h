/*!
    \file benchmark_threads.h
    \brief Threads benchmark base definition
    \author Ivan Shynkarenka
    \date 21.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_BENCHMARK_THREADS_H
#define CPPBENCHMARK_BENCHMARK_THREADS_H

#include "benchmark/benchmark_base.h"
#include "benchmark/context_threads.h"
#include "benchmark/fixture_threads.h"
#include "benchmark/settings_threads.h"

#include <thread>

namespace CppBenchmark {

//! Threads benchmark base class
/*!
    Provides interface to perform benchmark in multi-thread environment.
*/
class BenchmarkThreads : public BenchmarkBase, public virtual FixtureThreads
{
public:
    //! BenchmarkThreads settings type
    typedef SettingsThreads TSettings;

    //! Default class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    template<typename... Types>
    explicit BenchmarkThreads(const std::string& name, Types... settings) : BenchmarkThreads(name, TSettings(settings...)) {}
    //! Alternative class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    explicit BenchmarkThreads(const std::string& name, const TSettings& settings) : BenchmarkBase(name, settings) {}
    BenchmarkThreads(const BenchmarkThreads&) = delete;
    BenchmarkThreads(BenchmarkThreads&&) = delete;
    virtual ~BenchmarkThreads() = default;

    BenchmarkThreads& operator=(const BenchmarkThreads&) = delete;
    BenchmarkThreads& operator=(BenchmarkThreads&&) = delete;

protected:
    //! Thread run method
    /*!
        Should be implemented to provide code which will be executed in separate thread!

        \param context - Threads running context
    */
    virtual void RunThread(ContextThreads& context) = 0;

private:
    std::vector<std::thread> _threads;

    int CountLaunches() const override;
    void Launch(int& current, int total, LauncherHandler& handler) override;
};

/*! \example threads.cpp Threads integer increment benchmark */

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_THREADS_H
