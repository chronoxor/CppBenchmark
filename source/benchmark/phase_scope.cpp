/*!
    \file phase_scope.cpp
    \brief Benchmark phase scope implementation
    \author Ivan Shynkarenka
    \date 02.07.2015
    \copyright MIT License
*/

#include "benchmark/phase_scope.h"

namespace CppBenchmark {

const std::string PhaseScope::EMPTY_NAME("<none>");
const PhaseMetrics PhaseScope::EMPTY_METRICS;

PhaseScope::~PhaseScope()
{
    if (_phase)
    {
        _phase->StopPhase();
        _phase.reset();
    }
}

std::shared_ptr<Phase> PhaseScope::StartPhase(const std::string& phase)
{
    return _phase ? _phase->StartPhase(phase) : nullptr;
}

std::shared_ptr<Phase> PhaseScope::StartPhaseThreadSafe(const std::string& phase)
{
    return _phase ? _phase->StartPhaseThreadSafe(phase) : nullptr;
}

void PhaseScope::StopPhase()
{
    if (_phase)
    {
        _phase->StopPhase();
        _phase.reset();
    }
}

std::shared_ptr<PhaseScope> PhaseScope::ScopePhase(const std::string& phase)
{
    return _phase ? _phase->ScopePhase(phase) : nullptr;
}

std::shared_ptr<PhaseScope> PhaseScope::ScopePhaseThreadSafe(const std::string& phase)
{
    return _phase ? _phase->ScopePhaseThreadSafe(phase) : nullptr;
}

} // namespace CppBenchmark
