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
#include "benchmark/fixture.h"

namespace CppBenchmark {

//! Producers/Consumers benchmark context fixture
/*!
    Provides interface to initialize and cleanup producers/consumers benchmarks.
*/
template <class TContext>
class ContextFixturePC : public virtual ContextFixture<TContext>
{
public:
    ContextFixturePC() noexcept = default;
    ContextFixturePC(const ContextFixturePC&) noexcept = default;
    ContextFixturePC(ContextFixturePC&&) noexcept = default;
    virtual ~ContextFixturePC() noexcept = default;

    ContextFixturePC& operator=(const ContextFixturePC&) noexcept = default;
    ContextFixturePC& operator=(ContextFixturePC&&) noexcept = default;

protected:
    //! Initialize producer
    /*!
        This method is called to initialize producer in separate thread.

        \param context - Producer running context
    */
    virtual void InitializeProducer(TContext& context) {}
    //! Initialize consumer
    /*!
        This method is called to initialize consumer in separate thread.

        \param context - Consumer running context
    */
    virtual void InitializeConsumer(TContext& context) {}
    //! Cleanup producer
    /*!
        This method is called to cleanup producer in separate thread.

        \param context - Producer running context
    */
    virtual void CleanupProducer(TContext& context) {}
    //! Cleanup consumer
    /*!
        This method is called to cleanup consumer in separate thread.

        \param context - Consumer running context
    */
    virtual void CleanupConsumer(TContext& context) {}
};

//! Producers/Consumers benchmark fixture
typedef ContextFixturePC<ContextPC> FixturePC;

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_PC_H
