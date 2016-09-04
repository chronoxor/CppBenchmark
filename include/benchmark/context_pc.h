/*!
    \file context_pc.h
    \brief Benchmark producer/consumer running context definition
    \author Ivan Shynkarenka
    \date 24.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_CONTEXT_PC_H
#define CPPBENCHMARK_CONTEXT_PC_H

#include "benchmark/context.h"

namespace CppBenchmark {

//! Benchmark producer/consumer running context
/*!
    Provides access to the current producer/consumer benchmark context information:
    - Benchmark parameters (x, y, z)
    - Benchmark producers/consumers count
    - Benchmark metrics
    - Benchmark cancellation methods
    - Benchmark phase management methods
    - Benchmark producers/consumers management methods
*/
class ContextPC : public Context
{
    friend class BenchmarkPC;

public:
    ContextPC() = delete;
    ContextPC(const ContextPC&) noexcept = default;
    ContextPC(ContextPC&&) noexcept = default;
    virtual ~ContextPC() noexcept = default;

    ContextPC& operator=(const ContextPC&) noexcept = default;
    ContextPC& operator=(ContextPC&&) noexcept = default;

    //! Benchmark producers count
    int producers() const noexcept { return _producers; }
    //! Benchmark consumers count
    int consumers() const noexcept { return _consumers; }

    //! Is benchmark produce stopped?
    bool produce_stopped() const noexcept { return *_produce_stopped; }
    //! Is benchmark consume stopped?
    bool consume_stopped() const noexcept { return *_consume_stopped; }
    //! Stop benchmark production
    void StopProduce() noexcept { *_produce_stopped = true; }
    //! Stop benchmark consumption
    void StopConsume() noexcept { *_consume_stopped = true; }

    //! Get description of the current benchmark running context
    std::string description() const override;

protected:
    //! Benchmark producers count
    int _producers;
    //! Benchmark consumers count
    int _consumers;
    //! Benchmark produce stopped flag
    std::shared_ptr<std::atomic<bool>> _produce_stopped;
    //! Benchmark consume stopped flag
    std::shared_ptr<std::atomic<bool>> _consume_stopped;

    //! Create benchmark running context
    /*!
        \param producers - Benchmark producers count
        \param consumers - Benchmark consumers count
        \param x - Benchmark first parameter
        \param y - Benchmark second parameter
        \param z - Benchmark third parameter
    */
    ContextPC(int producers, int consumers, int x, int y, int z) noexcept
        : Context(x, y, z),
          _producers(producers),
          _consumers(consumers),
          _produce_stopped(std::make_shared<std::atomic<bool>>(false)),
          _consume_stopped(std::make_shared<std::atomic<bool>>(false))
    {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_PC_H
