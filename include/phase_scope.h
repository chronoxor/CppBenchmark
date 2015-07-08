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

    explicit operator bool() const noexcept { return _ptr_phase ? true : false; }

    // Implementation of Phase
    virtual const std::string& name() const { return _ptr_phase ? _ptr_phase->name() : EMPTY_NAME; }
    virtual const PhaseMetrics& metrics() const { return _ptr_phase ? _ptr_phase->metrics() : EMPTY_METRICS; }
    virtual PhaseMetrics& metrics() { return _ptr_phase ? _ptr_phase->metrics() : EMPTY_METRICS; }
    virtual std::shared_ptr<Phase> Start(const std::string& phase);
    virtual void Stop();
    virtual std::shared_ptr<PhaseScope> Scope(const std::string& phase);

private:
    std::shared_ptr<Phase> _ptr_phase;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_SCOPE_H
