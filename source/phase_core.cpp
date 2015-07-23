//
// Created by Ivan Shynkarenka on 03.07.2015.
//

#include "phase_core.h"
#include "system.h"

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
    result->StartIteration();

    return result;
}

std::shared_ptr<Phase> PhaseCore::StartPhaseThreadSafe(const std::string& phase)
{
    std::shared_ptr<PhaseCore> result;

    // Update phase collection under lock guard...
    {
        std::lock_guard<std::mutex> lock(_mutex);

        // Find or create a sub phase with the given name
        auto it = std::find_if(_child.begin(), _child.end(), [&phase](std::shared_ptr<PhaseCore>& item) { return ((item->name() == phase) && (item->_thread == System::CurrentThreadId())); });
        if (it == _child.end()) {
            result = std::make_shared<PhaseCore>(phase);
            _child.emplace_back(result);
        }
        else
            result = *it;
    }

    // Start new iteration for the child phase
    result->StartIteration();

    return result;
}

void PhaseCore::StopPhase()
{
    // End the current iteration for the current phase
    StopIteration();
}

void PhaseCore::StartIteration()
{
    _metrics.StartIteration();
}

void PhaseCore::StopIteration()
{
    _metrics.StopIteration();
}

void PhaseCore::Update()
{
    if (_metrics.total_time() < _best.total_time())
        _best = _metrics;
    if (_metrics.total_time() > _worst.total_time())
        _worst = _metrics;
    _metrics = PhaseMetrics();
}

} // namespace CppBenchmark
