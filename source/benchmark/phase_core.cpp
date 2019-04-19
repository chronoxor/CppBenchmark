/*!
    \file phase_core.cpp
    \brief Benchmark phase core implementation
    \author Ivan Shynkarenka
    \date 02.07.2015
    \copyright MIT License
*/

#include "benchmark/phase_core.h"

#include <algorithm>

namespace CppBenchmark {

std::shared_ptr<Phase> PhaseCore::StartPhase(const std::string& phase)
{
    std::shared_ptr<PhaseCore> result;

    // Find or create a sub phase with the given name
    auto it = std::find_if(_child.begin(), _child.end(), [&phase](const std::shared_ptr<PhaseCore>& item) { return item->name() == phase; });
    if (it == _child.end())
    {
        result = std::make_shared<PhaseCore>(phase);
        _child.emplace_back(result);
    }
    else
        result = *it;

    // Start new operation for the child phase
    result->StartCollectingMetrics();

    // Add new metrics operation
    result->_metrics_current.AddOperations(1);

    return result;
}

std::shared_ptr<Phase> PhaseCore::StartPhaseThreadSafe(const std::string& phase)
{
    std::shared_ptr<PhaseCore> result;

    // Update phase collection under lock guard...
    {
        std::scoped_lock lock(_mutex);

        // Find or create a sub phase with the given name
        auto it = std::find_if(_child.begin(), _child.end(), [&phase](const std::shared_ptr<PhaseCore>& item)
        {
            return ((item->name() == phase) && (item->_thread == System::CurrentThreadId()));
        });
        if (it == _child.end())
        {
            result = std::make_shared<PhaseCore>(phase);
            _child.emplace_back(result);
        }
        else
            result = *it;
    }

    // Start new operation for the child phase
    result->StartCollectingMetrics();

    // Add new metrics operation
    result->_metrics_current.AddOperations(1);

    return result;
}

} // namespace CppBenchmark
