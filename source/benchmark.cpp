//
// Created by Ivan Shynkarenka on 06.07.2015.
//

#include "benchmark.h"

namespace CppBenchmark {

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
            // Call launching notification
            onLaunching(*this, context, attempt);
            // Run benchmark with the current context
            int64_t iterations = _settings._iterations;
            int64_t nanoseconds = _settings._nanoseconds;
            while ((iterations >= 0) || (nanoseconds >= 0))
            {
                auto start = std::chrono::high_resolution_clock::now();
                Run(context);
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
