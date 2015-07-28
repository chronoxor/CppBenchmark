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
    auto it = std::find_if(_child.begin(), _child.end(),
                           [&phase](std::shared_ptr<PhaseCore>& item) { return item->name() == phase; });
    if (it == _child.end()) {
        result = std::make_shared<PhaseCore>(phase);
        _child.emplace_back(result);
    }
    else
        result = *it;

    // Start new iteration for the child phase
    result->StartCollectingMetrics();

    // Add new metrics iteration
    result->_metrics_current.AddIterations(1);

    return result;
}

std::shared_ptr<Phase> PhaseCore::StartPhaseThreadSafe(const std::string& phase)
{
    std::shared_ptr<PhaseCore> result;

    // Update phase collection under lock guard...
    {
        std::lock_guard<std::mutex> lock(_mutex);

        // Find or create a sub phase with the given name
        auto it = std::find_if(_child.begin(), _child.end(), [&phase](std::shared_ptr<PhaseCore>& item) {
            return ((item->name() == phase) && (item->_thread == System::CurrentThreadId()));
        });
        if (it == _child.end()) {
            result = std::make_shared<PhaseCore>(phase);
            _child.emplace_back(result);
        }
        else
            result = *it;
    }

    // Start new iteration for the child phase
    result->StartCollectingMetrics();

    // Add new metrics iteration
    result->_metrics_current.AddIterations(1);

    return result;
}

} // namespace CppBenchmark
