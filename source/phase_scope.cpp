//
// Created by Ivan Shynkarenka on 02.07.2015.
//

#include "phase_scope.h"

namespace CppBenchmark {

const std::string PhaseScope::EMPTY_NAME = "<none>";
PhaseMetrics PhaseScope::EMPTY_METRICS = PhaseMetrics();

PhaseScope::~PhaseScope()
{
    Stop();
}

std::shared_ptr<Phase> PhaseScope::Start(const std::string& phase)
{
    return _ptr_phase ? _ptr_phase->Start(phase) : nullptr;
}

void PhaseScope::Stop()
{
    if (_ptr_phase) {
        _ptr_phase->Stop();
        _ptr_phase.reset();
    }
}

std::shared_ptr<PhaseScope> PhaseScope::Scope(const std::string& phase)
{
    return _ptr_phase ? _ptr_phase->Scope(phase) : nullptr;
}

} // namespace CppBenchmark
