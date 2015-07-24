//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#include "benchmark_mpmc.h"

#include "launcher_handler.h"
#include "system.h"

namespace CppBenchmark {

void BenchmarkMPMC::Launch(LauncherHandler* handler)
{
    // Make several attempts of execution...
    for (int attempt = 1; attempt <= _settings.attempts(); ++attempt) {

        // Run benchmark at least for 1 producer and 1 consumer
        if (_settings_mpmc._mpmc.empty())
            _settings_mpmc._mpmc.emplace_back(1, 1);

        // Run benchmark at least once
        if (_settings._params.empty())
            _settings._params.emplace_back(-1, -1, -1);

        // Run benchmark for every producers/consumers pair
        for (auto mpmc : _settings_mpmc.mpmc()) {

            int producers = std::get<0>(mpmc);
            int consumers = std::get<1>(mpmc);

            // Run benchmark for every input parameter (single, pair, triple)
            for (auto param : _settings.params()) {

                // Prepare benchmark context
                ContextMPMC context(producers, consumers, std::get<0>(param), std::get<1>(param), std::get<2>(param));

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
                context._current->StartPhaseMetrics();
                context._current->StartIterationMetrics();

                // Start benchmark producers as futures
                for (int i = 0; i < producers; ++i) {
                    _futures.push_back(
                            std::async(std::launch::async,
                                       [this, &context, iterations, nanoseconds]()
                                       {
                                           int64_t producer_iterations = iterations;
                                           int64_t producer_nanoseconds = nanoseconds;

                                           // Clone producer context
                                           ContextMPMC producer_context(context);

                                           // Create and start thread safe phase
                                           std::shared_ptr<Phase> producer_phase = context.StartPhaseThreadSafe("producer");
                                           PhaseCore* producer_phase_core = dynamic_cast<PhaseCore*>(producer_phase.get());

                                           // Update producer context
                                           producer_context._current = producer_phase_core;
                                           producer_context._metrics = &producer_phase_core->metrics();

                                           // Call initialize producer method...
                                           InitializeProducer(producer_context);

                                           producer_context._current->StartPhaseMetrics();
                                           while (!producer_context.canceled() && ((producer_iterations > 0) || (producer_nanoseconds > 0)))
                                           {
                                               auto start = std::chrono::high_resolution_clock::now();

                                               // Run producer method...
                                               producer_context._current->StartIterationMetrics();
                                               RunProducer(producer_context);
                                               producer_context._current->StopIterationMetrics();

                                               auto stop = std::chrono::high_resolution_clock::now();

                                               // Decrement iteration counters
                                               producer_iterations -= 1;
                                               producer_nanoseconds -= std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
                                           }
                                           producer_context._current->StopPhaseMetrics();

                                           // Call cleanup producer method...
                                           CleanupProducer(producer_context);

                                           // Update thread safe phase metrics
                                           UpdateBenchmarkMetrics(*producer_context._current);

                                           // Stop thread safe phase
                                           producer_phase->StopPhase();
                                       }));
                }

                // Start benchmark consumers as futures
                for (int i = 0; i < consumers; ++i) {
                    _futures.push_back(
                            std::async(std::launch::async,
                                       [this, &context, iterations, nanoseconds]()
                                       {
                                           int64_t consumer_iterations = iterations;
                                           int64_t consumer_nanoseconds = nanoseconds;

                                           // Clone consumer context
                                           ContextMPMC consumer_context(context);

                                           // Create and start thread safe phase
                                           std::shared_ptr<Phase> consumer_phase = context.StartPhaseThreadSafe("consumer");
                                           PhaseCore* consumer_phase_core = dynamic_cast<PhaseCore*>(consumer_phase.get());

                                           // Update consumer context
                                           consumer_context._current = consumer_phase_core;
                                           consumer_context._metrics = &consumer_phase_core->metrics();

                                           // Call initialize consumer method...
                                           InitializeConsumer(consumer_context);

                                           consumer_context._current->StartPhaseMetrics();
                                           while (!consumer_context.canceled() && ((consumer_iterations > 0) || (consumer_nanoseconds > 0)))
                                           {
                                               auto start = std::chrono::high_resolution_clock::now();

                                               // Run consumer method...
                                               consumer_context._current->StartIterationMetrics();
                                               RunConsumer(consumer_context);
                                               consumer_context._current->StopIterationMetrics();

                                               auto stop = std::chrono::high_resolution_clock::now();

                                               // Decrement iteration counters
                                               consumer_iterations -= 1;
                                               consumer_nanoseconds -= std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
                                           }
                                           consumer_context._current->StopPhaseMetrics();

                                           // Call cleanup consumer method...
                                           CleanupConsumer(consumer_context);

                                           // Update thread safe phase metrics
                                           UpdateBenchmarkMetrics(*consumer_context._current);

                                           // Stop thread safe phase
                                           consumer_phase->StopPhase();
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
                context._current->StopIterationMetrics();
                context._current->StopPhaseMetrics();

                // Call cleanup benchmark method...
                Cleanup(context);

                // Call launched notification...
                handler->onLaunched(*this, context, attempt);

                // Update benchmark root phase metrics
                context._current->ChooseBestWorstMetrics();
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
