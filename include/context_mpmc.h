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

    std::string to_string() const override;

private:
    int _producers;
    int _consumers;

    ContextMPMC(int producers, int consumers, int x, int y, int z) noexcept
            : Context(x, y, z),
              _producers(producers),
              _consumers(consumers)
    {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_MPMC_H
