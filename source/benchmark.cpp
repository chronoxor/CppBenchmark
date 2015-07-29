/*!
    \file benchmark.cpp
    \brief Benchmark base class implementation
    \author Ivan Shynkarenka
    \date 06.07.2015
    \copyright MIT License
*/
#include "benchmark.h"

#include <algorithm>
#include <chrono>

#include "launcher_handler.h"

namespace CppBenchmark {

int Benchmark::CountLaunches() const
{
    return _settings.attempts() * (_settings.params().empty() ? 1 : (int)_settings.params().size());
}

void Benchmark::Launch(int& current, int total, LauncherHandler& handler)
{
    // Make several attempts of execution...
    for (int attempt = 1; attempt <= _settings.attempts(); ++attempt) {

        // Run benchmark at least once
        if (_settings._params.empty())
            _settings._params.emplace_back(-1, -1, -1);

        // Run benchmark for every input parameter (single, pair, triple)
        for (auto param : _settings.params()) {

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

            std::chrono::time_point<std::chrono::high_resolution_clock> start;
            std::chrono::time_point<std::chrono::high_resolution_clock> stop;

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
        UpdateBenchmarkMetrics();
    }

    // Update benchmark threads
    UpdateBenchmarkThreads();

    // Update benchmark names
    UpdateBenchmarkNames();

    // Update benchmark launched flag
    _launched = true;
}

void Benchmark::InitBenchmarkContext(Context& context)
{
    std::string name = _name + context.to_string();
    std::shared_ptr<PhaseCore> result;

    // Find or create root phase for the given context
    auto it = std::find_if(_phases.begin(), _phases.end(), [&name] (std::shared_ptr<PhaseCore>& item) { return item->name() == name; });
    if (it == _phases.end()) {
        result = std::make_shared<PhaseCore>(name);
        _phases.emplace_back(result);
    }
    else
        result = *it;

    // Update the current benchmark
    context._current = result.get();
    context._metrics = &result->current();
}

void Benchmark::UpdateBenchmarkMetrics()
{
    for (auto it = _phases.begin(); it != _phases.end(); ++it)
        UpdateBenchmarkMetrics(**it);
}

void Benchmark::UpdateBenchmarkMetrics(PhaseCore& phase)
{
    for (auto it = phase._child.begin(); it != phase._child.end(); ++it)
        UpdateBenchmarkMetrics(**it);
    phase.MergeMetrics();
    phase.ResetMetrics();
}

void Benchmark::UpdateBenchmarkThreads()
{
    for (auto it = _phases.begin(); it != _phases.end(); ++it)
        UpdateBenchmarkThreads(**it);
}

void Benchmark::UpdateBenchmarkThreads(PhaseCore& phase)
{
    // Merge child benchmark phases with a same name for different threads
    for (auto it = phase._child.begin(); it != phase._child.end(); ++it) {
        for (auto next = it + 1; next != phase._child.end(); ++next) {
            if ((*it)->name() == (*next)->name()) {

                // Merge metrics results
                (*it)->MergeMetrics(**next);

                // Append child phases
                (*it)->_child.insert((*it)->_child.end(), (*next)->_child.begin(), (*next)->_child.end());

                // Mark to delete
                (*next)->_thread = 0;
            }
        }
    }

    // Remove child phases maked to delete
    phase._child.erase(std::remove_if(phase._child.begin(), phase._child.end(), [](const std::shared_ptr<PhaseCore>& p) { return p->_thread == 0; }), phase._child.end());

    // Perform the same operation for child phases
    for (auto it = phase._child.begin(); it != phase._child.end(); ++it)
        UpdateBenchmarkThreads(**it);

    // Reset thread Id for the current phase
    phase._thread = 0;
}

void Benchmark::UpdateBenchmarkNames()
{
    for (auto it = _phases.begin(); it != _phases.end(); ++it)
        UpdateBenchmarkNames(**it, (*it)->name());
}

void Benchmark::UpdateBenchmarkNames(PhaseCore& phase, const std::string& name)
{
    for (auto it = phase._child.begin(); it != phase._child.end(); ++it)
        UpdateBenchmarkNames(**it, name + '.' + (*it)->name());
    phase._name = name;

}

} // namespace CppBenchmark
