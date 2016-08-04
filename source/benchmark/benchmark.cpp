/*!
    \file benchmark.cpp
    \brief Benchmark implementation
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
    return _settings.attempts() * (_settings.params().empty() ? 1 : (int)_settings.params().size());
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

} // namespace CppBenchmark
