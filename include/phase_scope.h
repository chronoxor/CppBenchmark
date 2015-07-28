//
// Created by Ivan Shynkarenka on 02.07.2015.
//

#ifndef CPPBENCHMARK_PHASE_SCOPE_H
#define CPPBENCHMARK_PHASE_SCOPE_H

#include "Phase.h"

namespace CppBenchmark {

class PhaseScope : public Phase
{
private:
    static const std::string EMPTY_NAME;
    static PhaseMetrics EMPTY_METRICS;

public:
    explicit PhaseScope(const std::shared_ptr<Phase>& ptr_phase) noexcept : _ptr_phase(ptr_phase) {}
    PhaseScope(const PhaseScope&) noexcept = default;
    PhaseScope(PhaseScope&& instance) noexcept = default;
    virtual ~PhaseScope();

    PhaseScope& operator=(const PhaseScope&) noexcept = default;
    PhaseScope& operator=(PhaseScope&&) noexcept = default;

    explicit operator bool() const noexcept
    { return _ptr_phase ? true : false; }

    // Implementation of Phase
    const std::string& name() const override
    { return _ptr_phase ? _ptr_phase->name() : EMPTY_NAME; }
    const PhaseMetrics& metrics() const override
    { return _ptr_phase ? _ptr_phase->metrics() : EMPTY_METRICS; }
    std::shared_ptr<Phase> StartPhase(const std::string& phase) override;
    std::shared_ptr<Phase> StartPhaseThreadSafe(const std::string& phase) override;
    void StopPhase() override;
    std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) override;
    std::shared_ptr<PhaseScope> ScopePhaseThreadSafe(const std::string& phase) override;

private:
    std::shared_ptr<Phase> _ptr_phase;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_SCOPE_H
