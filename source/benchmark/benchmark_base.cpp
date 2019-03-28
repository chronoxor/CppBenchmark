/*!
    \file benchmark_base.cpp
    \brief Benchmark base implementation
    \author Ivan Shynkarenka
    \date 06.07.2015
    \copyright MIT License
*/

#include "benchmark/benchmark_base.h"

#include <algorithm>

namespace CppBenchmark {

void BenchmarkBase::InitBenchmarkContext(Context& context)
{
    std::string name = _name + context.description();
    std::shared_ptr<PhaseCore> result;

    // Find or create root phase for the given context
    auto it = std::find_if(_phases.begin(), _phases.end(), [&name] (const std::shared_ptr<PhaseCore>& item) { return item->name() == name; });
    if (it == _phases.end())
    {
        result = std::make_shared<PhaseCore>(name);
        _phases.emplace_back(result);
    }
    else
        result = *it;

    // Update the current benchmark
    context._current = result.get();
    context._metrics = &result->current();
}

void BenchmarkBase::UpdateBenchmarkMetrics(std::vector<std::shared_ptr<PhaseCore>>& phases)
{
    for (const auto& phase : phases)
        UpdateBenchmarkMetrics(*phase);
}

void BenchmarkBase::UpdateBenchmarkMetrics(PhaseCore& phase)
{
    for (const auto& child : phase._child)
        UpdateBenchmarkMetrics(*child);
    phase.MergeMetrics();
    phase.ResetMetrics();
}

void BenchmarkBase::UpdateBenchmarkThreads(std::vector<std::shared_ptr<PhaseCore>>& phases)
{
    // Merge benchmark phases with a same name for different threads
    for (auto it = phases.begin(); it != phases.end(); ++it)
    {
        for (auto next = it + 1; next != phases.end(); ++next)
        {
            if ((*it)->name() == (*next)->name())
            {
                // Merge metrics results
                (*it)->MergeMetrics(**next);

                // Append child phases
                (*it)->_child.insert((*it)->_child.end(), (*next)->_child.begin(), (*next)->_child.end());

                // Mark to delete
                (*next)->_thread = 0;
            }
        }
    }

    // Remove phases marked to delete
    phases.erase(std::remove_if(phases.begin(), phases.end(), [](const std::shared_ptr<PhaseCore>& p) { return p->_thread == 0; }), phases.end());

    // Perform the same operation for child phases
    for (const auto& phase : phases)
        UpdateBenchmarkThreads(phase->_child);
}

void BenchmarkBase::UpdateBenchmarkNames(std::vector<std::shared_ptr<PhaseCore>>& phases)
{
    for (const auto& phase : phases)
        UpdateBenchmarkNames(*phase, phase->name());
}

void BenchmarkBase::UpdateBenchmarkNames(PhaseCore& phase, const std::string& name)
{
    for (const auto& child : phase._child)
        UpdateBenchmarkNames(*child, name + "." + child->name());
    phase._name = name;
}

void BenchmarkBase::UpdateBenchmarkOperations(std::vector<std::shared_ptr<PhaseCore>>& phases)
{
    for (const auto& phase : phases)
        UpdateBenchmarkOperations(*phase);
}

void BenchmarkBase::UpdateBenchmarkOperations(PhaseCore& phase)
{
    phase._metrics_result.AddOperations(-1);
    for (const auto& child : phase._child)
        phase._metrics_result.AddOperations(child->metrics().threads() * child->metrics().total_operations());
}

} // namespace CppBenchmark
