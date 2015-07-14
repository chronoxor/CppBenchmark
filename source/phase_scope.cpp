//
// Created by Ivan Shynkarenka on 02.07.2015.
//

#include "phase_scope.h"

namespace CppBenchmark {

const std::string PhaseScope::EMPTY_NAME = "<none>";
PhaseMetrics PhaseScope::EMPTY_METRICS = PhaseMetrics();

PhaseScope::~PhaseScope()
{
    StopPhase();
}

std::shared_ptr<Phase> PhaseScope::StartPhase(const std::string& phase)
{
    return _ptr_phase ? _ptr_phase->StartPhase(phase) : nullptr;
}

void PhaseScope::StopPhase()
{
    if (_ptr_phase) {
        _ptr_phase->StopPhase();
        _ptr_phase.reset();
    }
}

std::shared_ptr<PhaseScope> PhaseScope::ScopePhase(const std::string& phase)
{
    return _ptr_phase ? _ptr_phase->ScopePhase(phase) : nullptr;
}

} // namespace CppBenchmark
