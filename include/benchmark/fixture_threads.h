/*!
    \file fixture_threads.h
    \brief Threads benchmark fixture definition
    \author Ivan Shynkarenka
    \date 21.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_FIXTURE_THREADS_H
#define CPPBENCHMARK_FIXTURE_THREADS_H

#include "benchmark/context_threads.h"

namespace CppBenchmark {

//! Threads benchmark fixture
/*!
    Provides interface to initialize and cleanup threads benchmarks.
*/
class FixtureThreads
{
public:
    FixtureThreads() noexcept = default;
    FixtureThreads(const FixtureThreads&) noexcept = default;
    FixtureThreads(FixtureThreads&&) noexcept = default;
    virtual ~FixtureThreads() noexcept = default;

    FixtureThreads& operator=(const FixtureThreads&) noexcept = default;
    FixtureThreads& operator=(FixtureThreads&&) noexcept = default;

protected:
    //! Initialize benchmark
    /*!
        This method is called before benchmark run method.

        \param context - Benchmark running context
    */
    virtual void Initialize(ContextThreads& context) {}
    //! Cleanup benchmark
    /*!
        This method is called after benchmark run method.

        \param context - Benchmark running context
    */
    virtual void Cleanup(ContextThreads& context) {}

    //! Initialize thread
    /*!
        This method is called to initialize benchmark thread.

        \param context - Thread running context
    */
    virtual void InitializeThread(ContextThreads& context) {}
    //! Cleanup thread
    /*!
        This method is called to cleanup benchmark thread.

        \param context - Thread running context
    */
    virtual void CleanupThread(ContextThreads& context) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_THREADS_H
