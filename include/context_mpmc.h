//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#ifndef CPPBENCHMARK_CONTEXT_MPMC_H
#define CPPBENCHMARK_CONTEXT_MPMC_H

#include "context.h"

namespace CppBenchmark {

class ContextMPMC : public Context
{
    friend class BenchmarkMPMC;

public:
    ContextMPMC() = delete;
    ContextMPMC(const ContextMPMC&) noexcept = default;
    ContextMPMC(ContextMPMC&&) noexcept = default;
    ~ContextMPMC() noexcept = default;

    ContextMPMC& operator=(const ContextMPMC&) noexcept = default;
    ContextMPMC& operator=(ContextMPMC&&) noexcept = default;

    int producers() const noexcept { return _producers; }
    int consumers() const noexcept { return _consumers; }

    bool produce_stopped() const noexcept { return *_produce_stopped; }
    bool consume_stopped() const noexcept { return *_consume_stopped; }
    void StopProduce() noexcept { *_produce_stopped = true; }
    void StopConsume() noexcept { *_consume_stopped = true; }

    std::string to_string() const override;

private:
    int _producers;
    int _consumers;
    std::shared_ptr<std::atomic<bool>> _produce_stopped;
    std::shared_ptr<std::atomic<bool>> _consume_stopped;

    ContextMPMC(int producers, int consumers, int x, int y, int z) noexcept
            : Context(x, y, z),
              _producers(producers),
              _consumers(consumers),
              _produce_stopped(std::make_shared<std::atomic<bool>>(false)),
              _consume_stopped(std::make_shared<std::atomic<bool>>(false))
    {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_MPMC_H
