//
// Created by Ivan Shynkarenka on 21.07.2015.
//

#include "benchmark_threads.h"

#include "launcher_handler.h"
#include "system.h"

namespace CppBenchmark {

void BenchmarkThreads::Launch(LauncherHandler* handler)
{
    // Make several attempts of execution...
    for (int attempt = 1; attempt <= _settings.attempts(); ++attempt) {

        // Run benchmark at least for N threads where N is hardware core count
        if (_settings_threads._threads.empty())
            _settings_threads._threads.emplace_back(System::CpuPhysicalCores());

        // Run benchmark at least once
        if (_settings._params.empty())
            _settings._params.emplace_back(-1, -1, -1);

        // Run benchmark for every threads count
        for (auto threads : _settings_threads.threads()) {

            // Run benchmark for every input parameter (single, pair, triple)
            for (auto param : _settings.params()) {

                // Prepare benchmark context
                ContextThread context(threads, std::get<0>(param), std::get<1>(param), std::get<2>(param));

                // Initialize the current benchmark
                InitBenchmarkContext(context);

                // Call launching notification...
                handler->onLaunching(*this, context, attempt);

                // Call initialize benchmark method...
                Initialize(context);

                // Run benchmark with the current context
                int64_t iterations = _settings.iterations();
                int64_t nanoseconds = _settings.nanoseconds();

                // Start benchmark root phase iteration
                context._current->StartIteration();
                context._metrics->AddIterations(1);

                // Start benchmark threads as futures
                for (int i = 0; i < threads; ++i) {
                    _futures.push_back(
                            std::async(std::launch::async,
                                       [this, &context, threads, iterations, nanoseconds]()
                                       {
                                           int64_t thread_iterations = iterations;
                                           int64_t thread_nanoseconds = nanoseconds;

                                           // Clone thread context
                                           ContextThread thread_context(context);

                                           // Create and start thread safe phase
                                           std::shared_ptr<Phase> thread_phase = context.StartPhaseThreadSafe("thread");
                                           PhaseCore* thread_phase_core = dynamic_cast<PhaseCore*>(thread_phase.get());

                                           // Update thread context
                                           thread_context._current = thread_phase_core;
                                           thread_context._metrics = &thread_phase_core->metrics();

                                           // Call initialize thread method...
                                           InitializeThread(thread_context);

                                           thread_phase_core->StartIteration();
                                           while (!thread_context.canceled() && ((thread_iterations > 0) || (thread_nanoseconds > 0)))
                                           {
                                               // Add new metrics iteration
                                               thread_context._metrics->AddIterations(1);

                                               auto start = std::chrono::high_resolution_clock::now();

                                               // Run thread method...
                                               RunThread(thread_context);

                                               auto stop = std::chrono::high_resolution_clock::now();

                                               // Decrement iteration counters
                                               thread_iterations -= 1;
                                               thread_nanoseconds -= std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
                                           }
                                           thread_phase_core->StopIteration();

                                           // Call cleanup thread method...
                                           CleanupThread(thread_context);

                                           // Update thread safe phase metrics
                                           UpdateBenchmarkMetrics(*thread_phase_core);

                                           // Stop thread safe phase
                                           thread_phase->StopPhase();
                                       }));
                }

                // Wait for all futures
                for (auto& future : _futures) {
                    if (future.valid()) {
                        future.wait();
                    }
                };

                // Clear futures collection
                _futures.clear();

                // Stop benchmark root phase iteration
                context._current->StopIteration();

                // Call cleanup benchmark method...
                Cleanup(context);

                // Call launched notification...
                handler->onLaunched(*this, context, attempt);

                // Update benchmark root phase metrics
                context._current->Update();
            }
        }
    }

    // Update benchmark threads
    UpdateBenchmarkThreads();

    // Update benchmark names
    UpdateBenchmarkNames();

    // Update benchmark launched flag
    _launched = true;
}

} // namespace CppBenchmark
