/*!
    \file fixture_pc.h
    \brief Producers/Consumers benchmark fixture definition
    \author Ivan Shynkarenka
    \date 24.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_FIXTURE_PC_H
#define CPPBENCHMARK_FIXTURE_PC_H

#include "benchmark/context_pc.h"

namespace CppBenchmark {

//! Producers/Consumers benchmark fixture
/*!
    Provides interface to initialize and cleanup producers/consumers benchmarks.
*/
class FixturePC
{
public:
    FixturePC() noexcept = default;
    FixturePC(const FixturePC&) noexcept = default;
    FixturePC(FixturePC&&) noexcept = default;
    virtual ~FixturePC() noexcept = default;

    FixturePC& operator=(const FixturePC&) noexcept = default;
    FixturePC& operator=(FixturePC&&) noexcept = default;

protected:
    //! Initialize benchmark
    /*!
        This method is called before benchmark run method.

        \param context - Benchmark running context
    */
    virtual void Initialize(ContextPC& context) {}
    //! Cleanup benchmark
    /*!
        This method is called after benchmark run method.

        \param context - Benchmark running context
    */
    virtual void Cleanup(ContextPC& context) {}

    //! Initialize producer
    /*!
        This method is called to initialize producer in separate thread.

        \param context - Producer running context
    */
    virtual void InitializeProducer(ContextPC& context) {}
    //! Initialize consumer
    /*!
        This method is called to initialize consumer in separate thread.

        \param context - Consumer running context
    */
    virtual void InitializeConsumer(ContextPC& context) {}
    //! Cleanup producer
    /*!
        This method is called to cleanup producer in separate thread.

        \param context - Producer running context
    */
    virtual void CleanupProducer(ContextPC& context) {}
    //! Cleanup consumer
    /*!
        This method is called to cleanup consumer in separate thread.

        \param context - Consumer running context
    */
    virtual void CleanupConsumer(ContextPC& context) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_PC_H
