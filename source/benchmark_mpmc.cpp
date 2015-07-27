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
        if (_settings._mpmc.empty())
            _settings._mpmc.emplace_back(1, 1);

        // Run benchmark at least once
        if (_settings._params.empty())
            _settings._params.emplace_back(-1, -1, -1);

        // Run benchmark for every producers/consumers pair
        for (auto mpmc : _settings.mpmc()) {

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

                bool infinite = _settings.infinite();
                int64_t iterations = _settings.iterations();

                // Start benchmark root phase iteration
                context._current->StartCollectingMetrics();
                context._metrics->AddIterations(1);

                // Start benchmark producers as futures
                for (int i = 0; i < producers; ++i) {
                    _threads.push_back(std::thread([this, &context, infinite, iterations, i]()
                    {
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

                        bool producer_infinite = infinite;
                        int64_t producer_iterations = iterations;

                        std::chrono::time_point<std::chrono::high_resolution_clock> producer_start;
                        std::chrono::time_point<std::chrono::high_resolution_clock> producer_stop;

                        producer_context._current->StartCollectingMetrics();
                        while (!producer_context.produce_stopped() && !producer_context.canceled() && (producer_infinite || (producer_iterations > 0)))
                        {
                            // Add new metrics iteration
                            producer_context._metrics->AddIterations(1);

                            // Run producer method...
                            RunProducer(producer_context);

                            // Decrement iteration counters
                            producer_iterations -= 1;
                        }
                        producer_context._current->StopCollectingMetrics();

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
                    _threads.push_back(std::thread([this, &context, i]()
                    {
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

                        std::chrono::time_point<std::chrono::high_resolution_clock> consumer_start;
                        std::chrono::time_point<std::chrono::high_resolution_clock> consumer_stop;

                        consumer_context._current->StartCollectingMetrics();
                        while (!consumer_context.consume_stopped() && !consumer_context.canceled())
                        {
                            // Add new metrics iteration
                            consumer_context._metrics->AddIterations(1);

                            // Run consumer method...
                            RunConsumer(consumer_context);
                        }
                        consumer_context._current->StopCollectingMetrics();

                        // Call cleanup consumer method...
                        CleanupConsumer(consumer_context);

                        // Update thread safe phase metrics
                        UpdateBenchmarkMetrics(*consumer_context._current);

                        // Stop thread safe phase
                        consumer_phase->StopPhase();
                    }));
                }

                // Wait for all threads
                for (auto& thread : _threads) {
                    thread.join();
                };

                // Clear threads collection
                _threads.clear();

                // Stop benchmark root phase iteration
                context._current->StopCollectingMetrics();

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
