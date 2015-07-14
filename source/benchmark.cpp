//
// Created by Ivan Shynkarenka on 06.07.2015.
//

#include "benchmark.h"

#include <algorithm>

namespace CppBenchmark {

PhaseMetrics Benchmark::EMPTY_METRICS = PhaseMetrics();

void Benchmark::ResetCurrentBenchmark()
{
    // Reset the current benchmark
    _current.reset();
}

void Benchmark::UpdateCurrentBenchmark(const Context& context)
{
    std::string name = _name + to_string(context);
    std::shared_ptr<PhaseCore> result;

    // Find or create root phase for the given context
    auto it = std::find_if(_benchmarks.begin(), _benchmarks.end(), [&name] (std::shared_ptr<PhaseCore>& item) { return item->name() == name; });
    if (it == _benchmarks.end()) {
        result = std::make_shared<PhaseCore>(name);
        _benchmarks.emplace_back(result);
    }
    else
        result = *it;

    // Update the current benchmark
    _current = result;
}

void Benchmark::ResetMetrics()
{
    for (auto it = _benchmarks.begin(); it != _benchmarks.end(); ++it)
        ResetMetrics(**it);
}

void Benchmark::ResetMetrics(PhaseCore& phase)
{
    for (auto it = phase._child.begin(); it != phase._child.end(); ++it)
        ResetMetrics(**it);
    phase._metrics = PhaseMetrics();
}

void Benchmark::ChooseBestMetrics()
{
    for (auto it = _benchmarks.begin(); it != _benchmarks.end(); ++it)
        ChooseBestMetrics(**it);
}

void Benchmark::ChooseBestMetrics(PhaseCore& phase)
{
    for (auto it = phase._child.begin(); it != phase._child.end(); ++it)
        ChooseBestMetrics(**it);
    phase._best = (phase._metrics.total_time() < phase._best.total_time()) ? phase._metrics : phase._best;
}

void Benchmark::UpdateFinalMetrics()
{
    for (auto it = _benchmarks.begin(); it != _benchmarks.end(); ++it)
        UpdateFinalMetrics((*it)->name(), **it);
}

void Benchmark::UpdateFinalMetrics(const std::string& name, PhaseCore& phase)
{
    for (auto it = phase._child.begin(); it != phase._child.end(); ++it)
        UpdateFinalMetrics(name + '.' + (*it)->name(), **it);
    phase._name = name;
    phase._metrics = phase._best;
}

void Benchmark::Launch(/*std::function<void (const Benchmark&, const Context&, int)> onLaunching,
                       std::function<void (const Benchmark&, const Context&, int)> onLaunched*/)
{
    // Make several attempts of execution...
    for (int attempt = 0; attempt < _settings._attempts; ++attempt) {

        // Reset metrics for the current attempt
        ResetMetrics();

        // Initialize benchmark
        Initialize();

        // Run benchmark at least once
        if (_settings._params.empty())
            _settings._params.emplace_back(-1, -1, -1);

        // Run benchmark for every input parameter (single, pair, triple)
        for (auto param : _settings._params) {

            // Prepare benchmark context
            Context context(std::get<0>(param), std::get<1>(param), std::get<2>(param));

            // Get or create the current benchmark
            UpdateCurrentBenchmark(context);

            // Call launching notification
            //onLaunching(*this, context, attempt);

            // Run benchmark with the current context
            int64_t iterations = _settings._iterations;
            int64_t nanoseconds = _settings._nanoseconds;

            // Special check for default settings
            if ((iterations == 0) && (nanoseconds == 0))
                iterations = 1;

            while ((iterations > 0) || (nanoseconds > 0))
            {
                auto start = std::chrono::high_resolution_clock::now();

                _current->_metrics.StartIteration();
                Run(context);
                _current->_metrics.StopIteration();

                auto stop = std::chrono::high_resolution_clock::now();

                // Decrement iteration counters
                iterations -= 1;
                nanoseconds -= std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
            }

            // Call launched notification
            //onLaunched(*this, context, attempt);

            // Reset the current benchmark
            ResetCurrentBenchmark();
        }

        // Cleanup benchmark
        Cleanup();

        // Choose best metrics for the current attempt
        ChooseBestMetrics();
    }

    // Update final metrics
    UpdateFinalMetrics();
}

} // namespace CppBenchmark
