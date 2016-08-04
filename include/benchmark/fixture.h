/*!
    \file fixture.h
    \brief Benchmark fixture definition
    \author Ivan Shynkarenka
    \date 18.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_FIXTURE_H
#define CPPBENCHMARK_FIXTURE_H

#include "benchmark/context.h"

namespace CppBenchmark {

//! Benchmark context fixture
/*!
    Provides interface to initialize and cleanup benchmarks.
*/
template <class TContext>
class ContextFixture
{
public:
    ContextFixture() noexcept = default;
    ContextFixture(const ContextFixture&) noexcept = default;
    ContextFixture(ContextFixture&&) noexcept = default;
    virtual ~ContextFixture() noexcept = default;

    ContextFixture& operator=(const ContextFixture&) noexcept = default;
    ContextFixture& operator=(ContextFixture&&) noexcept = default;

protected:
    //! Initialize benchmark
    /*!
        This method is called before benchmark run method.

        \param context - Benchmark running context
    */
    virtual void Initialize(TContext& context) {}
    //! Cleanup benchmark
    /*!
        This method is called after benchmark run method.

        \param context - Benchmark running context
    */
    virtual void Cleanup(TContext& context) {}
};

//! Benchmark fixture
typedef ContextFixture<Context> Fixture;

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_H
