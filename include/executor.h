/*!
    \file executor.h
    \brief Dynamic benchmarks executor (not thread-safe) class definition
    \author Ivan Shynkarenka
    \date 02.09.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_EXECUTOR_H
#define CPPBENCHMARK_EXECUTOR_H

#include "phase_core.h"
#include "reporter.h"

namespace CppBenchmark {

//! Dynamic executor class. Not thread-safe!
/*!
    Provides interface to register dynamic benchmarks and report results with external reporters.

    Please note the class is not thread-safe and should not be used in multi-thread environment!
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
        and return it as a shared pointer.

        \param benchmark - Dynamic benchmark name
    */
    virtual std::shared_ptr<Phase> StartBenchmark(const std::string& benchmark);

    //! Start a new dynamic benchmark with a given name and wrap it in a PhaseScope
    /*!
        \param benchmark - Dynamic benchmark name
    */
    virtual std::shared_ptr<PhaseScope> ScopeBenchmark(const std::string& benchmark)
    { return std::make_shared<PhaseScope>(StartBenchmark(benchmark)); }

    //! Report benchmarks results using the given reporter
    /*!
        \param reporter - Reporter interface
    */
    virtual void Report(Reporter& reporter);

protected:
    //! Registered benchmarks collection
    std::vector<std::shared_ptr<PhaseCore>> _benchmarks;

private:
    void ReportPhase(Reporter& reporter, const PhaseCore& phase, const std::string& name);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_EXECUTOR_H
