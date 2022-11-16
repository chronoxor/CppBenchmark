/*!
    \file benchmark_pc.cpp
    \brief Producers/Consumers benchmark base implementation
    \author Ivan Shynkarenka
    \date 24.07.2015
    \copyright MIT License
*/

#include "benchmark/benchmark_pc.h"

#include "benchmark/barrier.h"
#include "benchmark/launcher_handler.h"

namespace CppBenchmark {

int BenchmarkPC::CountLaunches() const
{
    return _settings.attempts() * (_settings.pc().empty() ? 1 : (int)_settings.pc().size()) * (_settings.params().empty() ? 1 : (int)_settings.params().size());
}

void BenchmarkPC::Launch(int& current, int total, LauncherHandler& handler)
{
    // Make several attempts of execution...
    for (int attempt = 1; attempt <= _settings.attempts(); ++attempt)
    {
        // Run benchmark at least for 1 producer and 1 consumer
        if (_settings._pc.empty())
            _settings._pc.emplace_back(1, 1);

        // Run benchmark at least once
        if (_settings._params.empty())
            _settings._params.emplace_back(-1, -1, -1);

        // Run benchmark for every producers/consumers pair
        for (const auto& pc : _settings.pc())
        {
            int producers = std::get<0>(pc);
            int consumers = std::get<1>(pc);

            // Run benchmark for every input parameter (single, pair, triple)
            for (const auto& param : _settings.params())
            {
                // Prepare benchmark context
                ContextPC context(producers, consumers, std::get<0>(param), std::get<1>(param), std::get<2>(param));

                // Initialize the current benchmark
                InitBenchmarkContext(context);

                // Prepare latency histogram parameters
                std::tuple<int64_t, int64_t, int> latency_params(_settings.latency());
                bool latency_auto = _settings.latency_auto();

                // Call launching notification...
                handler.onLaunching(++current, total, *this, context, attempt);

                // Call initialize benchmark methods...
                Initialize(context);

                bool infinite = _settings.infinite();
                int64_t duration = _settings.duration();
                int64_t operations = _settings.operations();

                // Prepare barrier for producers & consumers threads
                Barrier barrier(producers + consumers);

                // Start benchmark root phase operation
                context._current->StartCollectingMetrics();
                context._metrics->AddOperations(1);

                // Start benchmark producers
                for (int i = 0; i < producers; ++i)
                {
                    _threads.emplace_back([this, &barrier, &context, latency_params, latency_auto, producers, infinite, operations, duration, i]()
                    {
                        // Clone producer context
                        ContextPC producer_context(context);

                        // Create and start thread safe phase
                        std::shared_ptr<Phase> producer_phase = context.StartPhaseThreadSafe("producer-" + std::to_string(i));
                        PhaseCore* producer_phase_core = dynamic_cast<PhaseCore*>(producer_phase.get());

                        // Update producer context
                        producer_context._current = producer_phase_core;
                        producer_context._metrics = &producer_phase_core->current();
                        producer_context._metrics->AddOperations(-1);
                        producer_context._metrics->SetThreads(producers);

                        // Initialize latency histogram of the current phase
                        producer_context._current->InitLatencyHistogram(latency_params);

                        // Call initialize producer method...
                        InitializeProducer(producer_context);

                        bool producer_infinite = infinite;
                        int64_t producer_duration = duration;
                        int64_t producer_operations = operations;

                        uint64_t timestamp = 0;

                        // Wait for other threads at the barrier
                        barrier.Wait();

                        // Calculate the approximate count of operations which can be performed for the given duration
                        if (producer_duration > 0)
                        {
                            uint64_t count = 0;
                            uint64_t timespan = 0;

                            // Collect data for one second...
                            for (; timespan < 1000000000; ++count)
                            {
                                timestamp = System::Timestamp();

                                // Run producer method...
                                RunProducer(producer_context);

                                timespan += System::Timestamp() - timestamp;
                            }

                            // Approximate operations count
                            producer_operations = (1000000000ull * count * producer_duration) / timespan;
                        }

                        producer_context._current->StartCollectingMetrics();
                        while (!producer_context.produce_stopped() && !producer_context.canceled() && (producer_infinite || (producer_operations > 0)))
                        {
                            // Add new metrics operation
                            producer_context._metrics->AddOperations(1);

                            // Store the timestamp for the automatic latency update
                            if (latency_auto)
                                timestamp = System::Timestamp();

                            // Run producer method...
                            RunProducer(producer_context);

                            // Update latency metrics
                            if (latency_auto)
                                producer_context._metrics->AddLatency(System::Timestamp() - timestamp);

                            // Decrement operation counters
                            producer_operations -= 1;
                        }
                        producer_context._current->StopCollectingMetrics();

                        // Call cleanup producer method...
                        CleanupProducer(producer_context);

                        // Update thread safe phase metrics
                        UpdateBenchmarkMetrics(*producer_context._current);
                    });
                }

                // Start benchmark consumers
                for (int i = 0; i < consumers; ++i)
                {
                    _threads.emplace_back([this, &barrier, &context, latency_params, latency_auto, consumers, i]()
                    {
                        // Clone consumer context
                        ContextPC consumer_context(context);

                        // Create and start thread safe phase
                        std::shared_ptr<Phase> consumer_phase = context.StartPhaseThreadSafe("consumer-" + std::to_string(i));
                        PhaseCore* consumer_phase_core = dynamic_cast<PhaseCore*>(consumer_phase.get());

                        // Update consumer context
                        consumer_context._current = consumer_phase_core;
                        consumer_context._metrics = &consumer_phase_core->current();
                        consumer_context._metrics->AddOperations(-1);
                        consumer_context._metrics->SetThreads(consumers);

                        // Initialize latency histogram of the current phase
                        consumer_context._current->InitLatencyHistogram(latency_params);

                        // Call initialize consumer method...
                        InitializeConsumer(consumer_context);

                        // Wait for other threads at the barrier
                        barrier.Wait();

                        consumer_context._current->StartCollectingMetrics();
                        while (!consumer_context.consume_stopped() && !consumer_context.canceled())
                        {
                            // Add new metrics operation
                            consumer_context._metrics->AddOperations(1);

                            // Store timestamp for automatic latency update
                            uint64_t timestamp = 0;
                            if (latency_auto)
                                timestamp = System::Timestamp();

                            // Run consumer method...
                            RunConsumer(consumer_context);

                            // Automatic latency update
                            if (latency_auto)
                                consumer_context._metrics->AddLatency(System::Timestamp() - timestamp);
                        }
                        consumer_context._current->StopCollectingMetrics();

                        // Call cleanup consumer method...
                        CleanupConsumer(consumer_context);

                        // Update thread safe phase metrics
                        UpdateBenchmarkMetrics(*consumer_context._current);
                    });
                }

                // Wait for all threads
                for (auto& thread : _threads)
                    thread.join();

                // Clear threads collection
                _threads.clear();

                // Stop benchmark root phase operation
                context._current->StopCollectingMetrics();

                // Call cleanup benchmark methods...
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
