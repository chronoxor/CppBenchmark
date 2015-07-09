//
// Created by Ivan Shynkarenka on 03.07.2015.
//

#include "phase_core.h"

namespace CppBenchmark {

std::shared_ptr<Phase> PhaseCore::Start(const std::string& phase)
{
    std::shared_ptr<PhaseCore> result;

    // Find or create a sub phase with the given name
    auto it = _child.find(phase);
    if (it == _child.end())
        result = _child.emplace(phase, std::make_shared<PhaseCore>(phase)).first->second;
    else
        result = it->second;

    // Start new iteration for the phase
    result->_metrics.StartIteration();

    return result;
}

void PhaseCore::Stop()
{
    // End the current iteration for the phase
    _metrics.StopIteration();
}

std::shared_ptr<PhaseScope> PhaseCore::Scope(const std::string& phase)
{
    return std::make_shared<PhaseScope>(Start(phase));
}

} // namespace CppBenchmark
