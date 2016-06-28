/*!
    \file fixture_threads.h
    \brief Threads benchmark fixture definition
    \author Ivan Shynkarenka
    \date 21.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_FIXTURE_THREADS_H
#define CPPBENCHMARK_FIXTURE_THREADS_H

#include "benchmark/context_thread.h"
#include "benchmark/fixture.h"

namespace CppBenchmark {

//! Threads benchmark fixture
/*!
    Provides interface to initialize and cleanup threads benchmarks.
*/
class FixtureThreads : public virtual Fixture
{
public:
    FixtureThreads() noexcept = default;
    FixtureThreads(const FixtureThreads&) noexcept = default;
    FixtureThreads(FixtureThreads&&) noexcept = default;
    virtual ~FixtureThreads() noexcept = default;

    FixtureThreads& operator=(const FixtureThreads&) noexcept = default;
    FixtureThreads& operator=(FixtureThreads&&) noexcept = default;

protected:
    //! Initialize thread
    /*!
        This method is called to initialize benchmark thread.

        \param context - Thread running context
    */
    virtual void InitializeThread(ContextThread& context) {}
    //! Cleanup thread
    /*!
        This method is called to cleanup benchmark thread.

        \param context - Thread running context
    */
    virtual void CleanupThread(ContextThread& context) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_THREADS_H
