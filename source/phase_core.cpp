//
// Created by Ivan Shynkarenka on 03.07.2015.
//

#include "phase_core.h"

#include <algorithm>

namespace CppBenchmark {

std::shared_ptr<Phase> PhaseCore::StartPhase(const std::string& phase)
{
    std::shared_ptr<PhaseCore> result;

    // Find or create a sub phase with the given name
    auto it = std::find_if(_child.begin(), _child.end(), [&phase] (std::shared_ptr<PhaseCore>& item) { return item->name() == phase; });
    if (it == _child.end()) {
        result = std::make_shared<PhaseCore>(phase);
        _child.emplace_back(result);
    }
    else
        result = *it;

    // Start new iteration for the child phase
    result->_metrics.StartIteration();

    return result;
}

void PhaseCore::StopPhase()
{
    // End the current iteration for the current phase
    _metrics.StopIteration();
}

std::shared_ptr<PhaseScope> PhaseCore::ScopePhase(const std::string& phase)
{
    return std::make_shared<PhaseScope>(StartPhase(phase));
}

} // namespace CppBenchmark
