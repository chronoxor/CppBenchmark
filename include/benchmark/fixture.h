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

//! Benchmark fixture
/*!
    Provides interface to initialize and cleanup benchmarks.
*/
class Fixture
{
public:
    Fixture() noexcept = default;
    Fixture(const Fixture&) noexcept = default;
    Fixture(Fixture&&) noexcept = default;
    virtual ~Fixture() noexcept = default;

    Fixture& operator=(const Fixture&) noexcept = default;
    Fixture& operator=(Fixture&&) noexcept = default;

protected:
    //! Initialize benchmark
    /*!
        This method is called before benchmark run method.

        \param context - Benchmark running context
    */
    virtual void Initialize(Context& context) {}
    //! Cleanup benchmark
    /*!
        This method is called after benchmark run method.

        \param context - Benchmark running context
    */
    virtual void Cleanup(Context& context) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_H
