/*!
    \file benchmark_threads.cpp
    \brief Threads benchmark base implementation
    \author Ivan Shynkarenka
    \date 21.07.2015
    \copyright MIT License
*/

#include "benchmark/benchmark_threads.h"

#include "benchmark/barrier.h"
#include "benchmark/launcher_handler.h"
#include "benchmark/system.h"

namespace CppBenchmark {

int BenchmarkThreads::CountLaunches() const
{
    return _settings.attempts() * (_settings.threads().empty() ? 1 : (int)_settings.threads().size()) * (_settings.params().empty() ? 1 : (int)_settings.params().size());
}

void BenchmarkThreads::Launch(int& current, int total, LauncherHandler& handler)
{
    // Make several attempts of execution...
    for (int attempt = 1; attempt <= _settings.attempts(); ++attempt)
    {
        // Run benchmark at least for N threads where N is hardware core count
        if (_settings._threads.empty())
            _settings._threads.emplace_back(System::CpuPhysicalCores());

        // Run benchmark at least once
        if (_settings._params.empty())
            _settings._params.emplace_back(-1, -1, -1);

        // Run benchmark for every threads count
        for (auto& threads : _settings.threads())
        {
            // Run benchmark for every input parameter (single, pair, triple)
            for (auto& param : _settings.params())
            {
                // Prepare benchmark context
                ContextThreads context(threads, std::get<0>(param), std::get<1>(param), std::get<2>(param));

                // Initialize the current benchmark
                InitBenchmarkContext(context);

                // Call launching notification...
                handler.onLaunching(++current, total, *this, context, attempt);

                // Call initialize benchmark methods...
                Initialize(context);

                bool infinite = _settings.infinite();
                int64_t iterations = _settings.iterations();

                // Prepare barrier for benchmark threads
                Barrier barrier(threads);

                // Start benchmark root phase iteration
                context._current->StartCollectingMetrics();
                context._metrics->AddIterations(1);

                // Start benchmark threads
                for (int i = 0; i < threads; ++i)
                {
                    _threads.push_back(std::thread([this, &barrier, &context, threads, infinite, iterations]()
                    {
                        // Clone thread context
                        ContextThreads thread_context(context);

                        // Create and start thread safe phase
                        std::shared_ptr<Phase> thread_phase = context.StartPhaseThreadSafe("thread");
                        PhaseCore* thread_phase_core = dynamic_cast<PhaseCore*>(thread_phase.get());

                        // Update thread context
                        thread_context._current = thread_phase_core;
                        thread_context._metrics = &thread_phase_core->current();
                        thread_context._metrics->AddIterations(-1);

                        // Call initialize thread method...
                        InitializeThread(thread_context);

                        bool thread_infinite = infinite;
                        int64_t thread_iterations = iterations;

                        // Wait for other threads at the barrier
                        barrier.Wait();

                        thread_context._current->StartCollectingMetrics();
                        while (!thread_context.canceled() && (thread_infinite || (thread_iterations > 0)))
                        {
                            // Add new metrics iteration
                            thread_context._metrics->AddIterations(1);

                            // Run thread method...
                            RunThread(thread_context);

                            // Decrement iteration counters
                            thread_iterations -= 1;
                        }
                        thread_context._current->StopCollectingMetrics();

                        // Call cleanup thread method...
                        CleanupThread(thread_context);

                        // Update thread safe phase metrics
                        UpdateBenchmarkMetrics(*thread_context._current);
                    }));
                }

                // Wait for all threads
                for (auto& thread : _threads)
                    thread.join();

                // Clear threads collection
                _threads.clear();

                // Stop benchmark root phase iteration
                context._current->StopCollectingMetrics();

                // Call cleanup benchmark method...
                Cleanup(context);

                // Call launched notification...
                handler.onLaunched(current, total, *this, context, attempt);

                // Update benchmark root metrics for the current attempt
                context._current->MergeMetrics();
                context._current->ResetMetrics();
            }
        }
    }

    // Update benchmark threads
    UpdateBenchmarkThreads(_phases);

    // Update benchmark names
    UpdateBenchmarkNames(_phases);

    // Update benchmark launched flag
    _launched = true;
}

} // namespace CppBenchmark
