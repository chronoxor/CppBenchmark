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
    FixtureThreads() = default;
    FixtureThreads(const FixtureThreads&) = default;
    FixtureThreads(FixtureThreads&&) = default;
    virtual ~FixtureThreads() = default;

    FixtureThreads& operator=(const FixtureThreads&) = default;
    FixtureThreads& operator=(FixtureThreads&&) = default;

protected:
    virtual void InitializeThread(ContextThread& context) {}
    virtual void CleanupThread(ContextThread& context) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_THREADS_H
