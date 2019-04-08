/*!
    \file benchmark_pc.h
    \brief Producers/Consumers benchmark base definition
    \author Ivan Shynkarenka
    \date 24.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_BENCHMARK_PC_H
#define CPPBENCHMARK_BENCHMARK_PC_H

#include "benchmark/benchmark_base.h"
#include "benchmark/context_pc.h"
#include "benchmark/fixture_pc.h"
#include "benchmark/settings_pc.h"

#include <thread>

namespace CppBenchmark {

//! Producers/Consumers benchmark base class
/*!
    Provides interface to perform benchmark of the producers/consumers pattern in multi-thread environment.
*/
class BenchmarkPC : public BenchmarkBase, public virtual FixturePC
{
public:
    //! BenchmarkPC settings type
    typedef SettingsPC TSettings;

    //! Default class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    template<typename... Types>
    explicit BenchmarkPC(const std::string& name, Types... settings) : BenchmarkPC(name, TSettings(settings...)) {}
    //! Alternative class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    explicit BenchmarkPC(const std::string& name, const TSettings& settings) : BenchmarkBase(name, settings) {}
    BenchmarkPC(const BenchmarkPC&) = delete;
    BenchmarkPC(BenchmarkPC&&) = delete;
    virtual ~BenchmarkPC() = default;

    BenchmarkPC& operator=(const BenchmarkPC&) = delete;
    BenchmarkPC& operator=(BenchmarkPC&&) = delete;

protected:
    //! Producer run method
    /*!
        Should be implemented to provide code which produces items. This code will be executed for each producer in
        separate thread!

        \param context - Producer running context
    */
    virtual void RunProducer(ContextPC& context) = 0;
    //! Consumer run method
    /*!
        Should be implemented to provide code which consumes items. This code will be executed for each consumer in
        separate thread!

        \param context - Consumers running context
    */
    virtual void RunConsumer(ContextPC& context) = 0;

private:
    std::vector<std::thread> _threads;

    int CountLaunches() const override;
    void Launch(int& current, int total, LauncherHandler& handler) override;
};

/*! \example spsc.cpp Single producer, single consumer benchmark */
/*! \example mpsc.cpp Multiple producers, single consumer benchmark */
/*! \example mpmc.cpp Multiple producers, multiple consumers benchmark */

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_PC_H
