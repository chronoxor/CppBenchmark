/*!
    \file benchmark.cpp
    \brief Benchmark implementation
    \author Ivan Shynkarenka
    \date 06.07.2015
    \copyright MIT License
*/

#include "benchmark/benchmark.h"

#include "benchmark/launcher_handler.h"

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

            // Initialize latency histogram of the current phase
            std::tuple<int64_t, int64_t, int> latency_params(_settings.latency());
            bool latency_auto = _settings.latency_auto();
            context._current->InitLatencyHistogram(latency_params);

            // Call launching notification...
            handler.onLaunching(++current, total, *this, context, attempt);

            // Call initialize benchmark method...
            Initialize(context);

            bool infinite = _settings.infinite();
            int64_t operations = _settings.operations();

            context._current->StartCollectingMetrics();
            while (!context.canceled() && (infinite || (operations > 0)))
            {
                // Add new metrics operation
                context._metrics->AddOperations(1);

                // Store timestamp for automatic latency update
                uint64_t timestamp = 0;
                if (latency_auto)
                    timestamp = System::Timestamp();

                // Run benchmark method...
                Run(context);

                // Automatic latency update
                if (latency_auto)
                    context._metrics->AddLatency(System::Timestamp() - timestamp);

                // Decrement operation counters
                operations -= 1;
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
