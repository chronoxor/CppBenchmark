//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#ifndef CPPBENCHMARK_FIXTURE_MPMC_H
#define CPPBENCHMARK_FIXTURE_MPMC_H

#include "context_mpmc.h"
#include "fixture.h"

namespace CppBenchmark {

class FixtureMPMC : public virtual Fixture
{
public:
    FixtureMPMC() noexcept = default;
    FixtureMPMC(const FixtureMPMC&) noexcept = default;
    FixtureMPMC(FixtureMPMC&&) noexcept = default;
    virtual ~FixtureMPMC() noexcept = default;

    FixtureMPMC& operator=(const FixtureMPMC&) noexcept = default;
    FixtureMPMC& operator=(FixtureMPMC&&) noexcept = default;

protected:
    virtual void InitializeProducer(ContextMPMC& context) {}
    virtual void InitializeConsumer(ContextMPMC& context) {}
    virtual void CleanupProducer(ContextMPMC& context) {}
    virtual void CleanupConsumer(ContextMPMC& context) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_MPMC_H
