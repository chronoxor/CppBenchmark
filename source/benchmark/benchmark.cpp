/*!
    \file benchmark.cpp
    \brief Benchmark base implementation
    \author Ivan Shynkarenka
    \date 06.07.2015
    \copyright MIT License
*/

#include "benchmark/benchmark.h"

#include "benchmark/launcher_handler.h"

#include <algorithm>

namespace CppBenchmark {

int Benchmark::CountLaunches() const
{
    return _settings.attempts() * (_settings.params().empty() ? 1 : static_cast<int>(_settings.params().size()));
}

void Benchmark::Launch(int& current, int total, LauncherHandler& handler)
{
    // Make several attempts of execution...
    for (int attempt = 1; attempt <= _settings.attempts(); ++attempt)
    {
        // Run benchmark at least once
        if (_settings._params.empty())
            _settings._params.emplace_back(-1, -1, -1);

        // Run benchmark for every input parameter (single, pair, triple)
        for (auto& param : _settings.params())
        {
            // Prepare benchmark context
            Context context(std::get<0>(param), std::get<1>(param), std::get<2>(param));

            // Initialize the current benchmark
            InitBenchmarkContext(context);

            // Call launching notification...
            handler.onLaunching(++current, total, *this, context, attempt);

            // Call initialize benchmark method...
            Initialize(context);

            bool infinite = _settings.infinite();
            int64_t iterations = _settings.iterations();

            context._current->StartCollectingMetrics();
            while (!context.canceled() && (infinite || (iterations > 0)))
            {
                // Add new metrics iteration
                context._metrics->AddIterations(1);

                // Run benchmark method...
                Run(context);

                // Decrement iteration counters
                iterations -= 1;
            }
            context._current->StopCollectingMetrics();

            // Call cleanup benchmark method...
            Cleanup(context);

            // Call launched notification...
            handler.onLaunched(current, total, *this, context, attempt);
        }

        // Update benchmark root metrics for the current attempt
        UpdateBenchmarkMetrics(_phases);
    }

    // Update benchmark threads
    UpdateBenchmarkThreads(_phases);

    // Update benchmark names
    UpdateBenchmarkNames(_phases);

    // Update benchmark launched flag
    _launched = true;
}

void Benchmark::InitBenchmarkContext(Context& context)
{
    std::string name = _name + context.to_string();
    std::shared_ptr<PhaseCore> result;

    // Find or create root phase for the given context
    auto it = std::find_if(_phases.begin(), _phases.end(), [&name] (std::shared_ptr<PhaseCore>& item) { return item->name() == name; });
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

void Benchmark::UpdateBenchmarkMetrics(std::vector<std::shared_ptr<PhaseCore>>& phases)
{
    for (auto& phase : phases)
        UpdateBenchmarkMetrics(*phase);
}

void Benchmark::UpdateBenchmarkMetrics(PhaseCore& phase)
{
    for (auto& child : phase._child)
        UpdateBenchmarkMetrics(*child);
    phase.MergeMetrics();
    phase.ResetMetrics();
}

void Benchmark::UpdateBenchmarkThreads(std::vector<std::shared_ptr<PhaseCore>>& phases)
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
    for (auto& phase : phases)
        UpdateBenchmarkThreads(phase->_child);
}

void Benchmark::UpdateBenchmarkNames(std::vector<std::shared_ptr<PhaseCore>>& phases)
{
    for (auto& phase : phases)
        UpdateBenchmarkNames(*phase, phase->name());
}

void Benchmark::UpdateBenchmarkNames(PhaseCore& phase, const std::string& name)
{
    for (auto& child : phase._child)
        UpdateBenchmarkNames(*child, name + "." + child->name());
    phase._name = name;
}

} // namespace CppBenchmark
