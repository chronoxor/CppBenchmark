/*!
    \file executor.h
    \brief Dynamic benchmarks executor class (thread-safe) definition
    \author Ivan Shynkarenka
    \date 02.09.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_EXECUTOR_THREADS_H
#define CPPBENCHMARK_EXECUTOR_THREADS_H

#include "executor.h"

namespace CppBenchmark {

//! Dynamic executor class. Thread-safe!
/*!
    Provides interface to register dynamic benchmarks and report results with external reporters.

    Please note the class is thread-safe and might be used in multi-thread environment!
*/
class ExecutorThreads : public Executor
{
public:
    ExecutorThreads() = default;
    ExecutorThreads(const ExecutorThreads&) = delete;
    ExecutorThreads(ExecutorThreads&&) = delete;
    virtual ~ExecutorThreads() = default;

    ExecutorThreads& operator=(const ExecutorThreads&) = delete;
    ExecutorThreads& operator=(ExecutorThreads&&) = delete;

    //! Get executor singleton instance
    static ExecutorThreads& GetInstance()
    { static ExecutorThreads instance; return instance; }

    // Implementation of Executor
    std::shared_ptr<Phase> StartBenchmark(const std::string& benchmark) override;
    void Report(Reporter& reporter) override;

protected:
    //! Synchronization mutex
    std::mutex _mutex;

private:
    void UpdateBenchmarkThreads();
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_EXECUTOR_THREADS_H
