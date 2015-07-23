//
// Created by Ivan Shynkarenka on 21.07.2015.
//

#ifndef CPPBENCHMARK_FIXTURE_THREADS_H
#define CPPBENCHMARK_FIXTURE_THREADS_H

#include "context_thread.h"
#include "fixture.h"

namespace CppBenchmark {

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
    virtual void InitializeThread(ContextThread&) {}
    virtual void CleanupThread(ContextThread&) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_THREADS_H
