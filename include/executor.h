/*!
    \file executor.h
    \brief Dynamic benchmarks executor class definition
    \author Ivan Shynkarenka
    \date 02.09.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_EXECUTOR_H
#define CPPBENCHMARK_EXECUTOR_H

#include "phase_core.h"
#include "reporter.h"

namespace CppBenchmark {

//! Dynamic executor class
/*!
    Provides interface to register dynamic benchmarks and report results with external reporters.
*/
class Executor
{
public:
    Executor() = default;
    Executor(const Executor&) = delete;
    Executor(Executor&&) = delete;
    virtual ~Executor() = default;

    Executor& operator=(const Executor&) = delete;
    Executor& operator=(Executor&&) = delete;

    //! Get executor singleton instance
    static Executor& GetInstance()
    { static Executor instance; return instance; }

    //! Start a new dynamic benchmark with a given name
    /*!
        This method will create or get existent dynamic benchmark with a given name, start benchmark measurement
        and return it as a shared pointer. Please note the method is thread-safe and might be called in multi-thread
        environment!

        \param benchmark - Dynamic benchmark name
    */
    std::shared_ptr<Phase> StartBenchmark(const std::string& benchmark);

    //! Stop dynamic benchmark with a given name
    /*!
        This method will try to find existent dynamic benchmark with a given name and stop benchmark measurement for it.
        Please note the method is thread-safe and might be called in multi-thread environment!

        \param benchmark - Dynamic benchmark name
    */
    void StopBenchmark(const std::string& benchmark);

    //! Start a new dynamic benchmark with a given name and wrap it in a PhaseScope
    /*!
        Please note the method is thread-safe and might be called in multi-thread environment!

        \param benchmark - Dynamic benchmark name
    */
    std::shared_ptr<PhaseScope> ScopeBenchmark(const std::string& benchmark)
    { return std::make_shared<PhaseScope>(StartBenchmark(benchmark)); }

    //! Report benchmarks results using the given reporter
    /*!
        Please note the method is thread-safe and might be called in multi-thread environment!

        \param reporter - Reporter interface
    */
    void Report(Reporter& reporter);

protected:
    //! Synchronization mutex
    std::mutex _mutex;
    //! Registered benchmarks collection
    std::vector<std::shared_ptr<PhaseCore>> _benchmarks;

private:
    void ReportPhase(Reporter& reporter, const PhaseCore& phase, const std::string& name);
};

/*! \example executor.cpp Dynamic benchmarks */

} // namespace CppBenchmark

#endif // CPPBENCHMARK_EXECUTOR_H
