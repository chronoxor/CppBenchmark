//
// Created by Ivan Shynkarenka on 06.07.2015.
//

#include "benchmark.h"

#include <algorithm>

namespace CppBenchmark {

std::shared_ptr<PhaseCore> Benchmark::UpdateCurrentBenchmark(const Context& context)
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

    return result;
}

void Benchmark::Launch(std::function<void (const Benchmark&, const Context&, int)> onLaunching,
                       std::function<void (const Benchmark&, const Context&, int)> onLaunched)
{
    // Make several attempts of execution...
    for (int attempt = 0; attempt < _settings._attempts; ++attempt) {
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
            onLaunching(*this, context, attempt);
            // Run benchmark with the current context
            int64_t iterations = _settings._iterations;
            int64_t nanoseconds = _settings._nanoseconds;
            while ((iterations >= 0) || (nanoseconds >= 0))
            {
                auto start = std::chrono::high_resolution_clock::now();
                _current->_metrics.StartIteration();
                Run(context);
                _current->_metrics.StopIteration();
                auto stop = std::chrono::high_resolution_clock::now();
                // Special check for single iteration launch (default settings)
                if ((iterations == 0) && (nanoseconds == 0))
                    break;
                // Decrement iteration counters
                iterations -= 1;
                nanoseconds -= std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
            }
            // Call launched notification
            onLaunched(*this, context, attempt);
        }

        // Cleanup benchmark
        Cleanup();
    }
}

} // namespace CppBenchmark
