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
        for (const auto& param : _settings.params())
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
            int64_t duration = _settings.duration();
            int64_t operations = _settings.operations();

            uint64_t timestamp = 0;

            // Calculate the approximate count of operations which can be performed for the given duration
            if (duration > 0)
            {
                uint64_t count = 0;
                uint64_t timespan = 0;

                // Collect data for one second...
                for (; timespan < 1000000000; ++count)
                {
                    timestamp = System::Timestamp();

                    // Run benchmark method...
                    Run(context);

                    timespan += System::Timestamp() - timestamp;
                }

                // Approximate operations count
                operations = (1000000000ull * count * duration) / timespan;
            }

            context._current->StartCollectingMetrics();
            while (!context.canceled() && (infinite || (operations > 0)))
            {
                // Add new metrics operation
                context._metrics->AddOperations(1);

                // Store the timestamp for the automatic latency update
                if (latency_auto)
                    timestamp = System::Timestamp();

                // Run benchmark method...
                Run(context);

                // Update latency metrics
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
