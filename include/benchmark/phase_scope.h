/*!
    \file phase_scope.h
    \brief Benchmark phase scope definition
    \author Ivan Shynkarenka
    \date 02.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_PHASE_SCOPE_H
#define CPPBENCHMARK_PHASE_SCOPE_H

#include "benchmark/phase.h"

namespace CppBenchmark {

//! Benchmark phase scope
/*!
    Implements scope guard pattern for benchmark phase which stops attached benchmark phase on destructing instance. It
    also provides base Phase interface.
*/
class PhaseScope : public Phase
{
public:
    //! Create benchmark phase scope and attach existing benchmark phase to it
    /*!
        \param phase - Attached benchmark phase
    */
    explicit PhaseScope(const std::shared_ptr<Phase>& phase) noexcept : _phase(phase) {}
    PhaseScope(const PhaseScope&) noexcept = default;
    PhaseScope(PhaseScope&&) noexcept = default;
    //! Benchmark phase will be stopped on destructing
    virtual ~PhaseScope();

    PhaseScope& operator=(const PhaseScope&) noexcept = default;
    PhaseScope& operator=(PhaseScope&&) noexcept = default;

    //! Is phase scope is valid?
    explicit operator bool() const noexcept
    { return _phase ? true : false; }

    // Implementation of Phase
    const std::string& name() const noexcept override
    { return _phase ? _phase->name() : EMPTY_NAME; }
    const PhaseMetrics& metrics() const noexcept override
    { return _phase ? _phase->metrics() : EMPTY_METRICS; }
    std::shared_ptr<Phase> StartPhase(const std::string& phase) override;
    std::shared_ptr<Phase> StartPhaseThreadSafe(const std::string& phase) override;
    void StopPhase() override;
    std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) override;
    std::shared_ptr<PhaseScope> ScopePhaseThreadSafe(const std::string& phase) override;

private:
    std::shared_ptr<Phase> _phase;

    static const std::string EMPTY_NAME;
    static const PhaseMetrics EMPTY_METRICS;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_SCOPE_H
