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

//! Threads benchmark context fixture
/*!
    Provides interface to initialize and cleanup threads benchmarks.
*/
template <class TContext>
class ContextFixtureThreads : public virtual ContextFixture<TContext>
{
public:
    ContextFixtureThreads() noexcept = default;
    ContextFixtureThreads(const ContextFixtureThreads&) noexcept = default;
    ContextFixtureThreads(ContextFixtureThreads&&) noexcept = default;
    virtual ~ContextFixtureThreads() noexcept = default;

    ContextFixtureThreads& operator=(const ContextFixtureThreads&) noexcept = default;
    ContextFixtureThreads& operator=(ContextFixtureThreads&&) noexcept = default;

protected:
    //! Initialize thread
    /*!
        This method is called to initialize benchmark thread.

        \param context - Thread running context
    */
    virtual void InitializeThread(TContext& context) {}
    //! Cleanup thread
    /*!
        This method is called to cleanup benchmark thread.

        \param context - Thread running context
    */
    virtual void CleanupThread(TContext& context) {}
};

//! Producers/Consumers benchmark fixture
typedef ContextFixtureThreads<ContextThread> FixtureThreads;

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_THREADS_H
