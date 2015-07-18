//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#include "launcher.h"

#include <regex>

namespace CppBenchmark {

void Launcher::MatchWithFilter(const std::string& pattern, std::function<void (Benchmark& benchmark)> action)
{
    std::regex matcher(pattern);

    for (auto benchmark : _benchmarks)
        if (pattern.empty() || std::regex_match(benchmark->name(), matcher))
            action(*benchmark);
}

void Launcher::LaunchWithFilter(const std::string& pattern)
{
    // Launch all suitable benchmarks
    MatchWithFilter(pattern, [this](Benchmark& benchmark) {
        LaunchBenchmark(benchmark);
    });

    // Report header, system & environment
    for (auto reporter : _reporters) {
        reporter->ReportHeader();
        reporter->ReportSystem();
        reporter->ReportEnvironment();
        reporter->ReportBenchmarksHeader();
    }

    // Report benchmark results
    MatchWithFilter(pattern, [this](Benchmark& benchmark) {
        for (auto reporter : _reporters) {
            reporter->ReportBenchmarkHeader();
            reporter->ReportBenchmark(benchmark, benchmark._settings);
            reporter->ReportPhasesHeader();
            for (auto root_phase : benchmark._phases) {
                ReportPhase(*reporter, *root_phase, root_phase->name());
            }
            reporter->ReportPhasesFooter();
            reporter->ReportBenchmarkFooter();
        }
    });

    // Report footer
    for (auto reporter : _reporters) {
        reporter->ReportBenchmarksFooter();
        reporter->ReportFooter();
    }
}

void Launcher::LaunchBenchmark(Benchmark& benchmark)
{
    // Make several attempts of execution...
    for (int attempt = 1; attempt <= benchmark._settings._attempts; ++attempt) {

        // Reset the current benchmark
        ResetBenchmark(benchmark);

        // Reset metrics for the current attempt
        ResetBenchmarkMetrics(benchmark);

        // Run benchmark at least once
        if (benchmark._settings._params.empty())
            benchmark._settings._params.emplace_back(-1, -1, -1);

        // Run benchmark for every input parameter (single, pair, triple)
        for (auto param : benchmark._settings._params) {

            // Prepare benchmark context
            Context context(std::get<0>(param), std::get<1>(param), std::get<2>(param));

            // Get or create the current benchmark
            UpdateBenchmark(benchmark, context);

            // Call launching notification
            onLaunching(benchmark, context, attempt);

            // Initialize benchmark
            benchmark.Initialize(context);

            // Run benchmark with the current context
            int64_t iterations = benchmark._settings._iterations;
            int64_t nanoseconds = benchmark._settings._nanoseconds;

            // Special check for default settings
            if ((iterations == 0) && (nanoseconds == 0))
                iterations = 1;

            while ((iterations > 0) || (nanoseconds > 0))
            {
                auto start = std::chrono::high_resolution_clock::now();

                benchmark._current->_metrics.StartIteration();
                benchmark.Run(context);
                benchmark._current->_metrics.StopIteration();

                auto stop = std::chrono::high_resolution_clock::now();

                // Decrement iteration counters
                iterations -= 1;
                nanoseconds -= std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start).count();
            }

            // Cleanup benchmark
            benchmark.Cleanup(context);

            // Call launched notification
            onLaunched(benchmark, context, attempt);

            // Reset the current benchmark
            ResetBenchmark(benchmark);
        }

        // Choose best metrics for the current attempt
        ChooseBestBenchmarkMetrics(benchmark);
    }

    // Update final metrics
    UpdateFinalBenchmarkMetrics(benchmark);
}

void Launcher::ReportPhase(Reporter& reporter, const PhaseCore& phase, const std::string& name)
{
    reporter.ReportPhaseHeader();
    reporter.ReportPhase(phase, phase.metrics());
    reporter.ReportPhaseFooter();
    for (auto child : phase._child) {
        std::string child_name = name + '.' + child->name();
        ReportPhase(reporter, *child, child_name);
    }
}

void Launcher::ResetBenchmark(Benchmark& benchmark)
{
    // Reset the current benchmark
    benchmark._current.reset();
}

void Launcher::UpdateBenchmark(Benchmark& benchmark, Context& context)
{
    std::string name = benchmark.name() + to_string(context);
    std::shared_ptr<PhaseCore> result;

    // Find or create root phase for the given context
    auto it = std::find_if(benchmark._phases.begin(), benchmark._phases.end(), [&name] (std::shared_ptr<PhaseCore>& item) { return item->name() == name; });
    if (it == benchmark._phases.end()) {
        result = std::make_shared<PhaseCore>(name);
        benchmark._phases.emplace_back(result);
    }
    else
        result = *it;

    // Update the current benchmark
    benchmark._current = result;
    context._current = result;
}

void Launcher::ResetBenchmarkMetrics(Benchmark& benchmark)
{
    for (auto it = benchmark._phases.begin(); it != benchmark._phases.end(); ++it)
        ResetBenchmarkMetrics(**it);
}

void Launcher::ResetBenchmarkMetrics(PhaseCore& phase)
{
    for (auto it = phase._child.begin(); it != phase._child.end(); ++it)
        ResetBenchmarkMetrics(**it);
    phase._metrics = PhaseMetrics();
}

void Launcher::ChooseBestBenchmarkMetrics(Benchmark& benchmark)
{
    for (auto it = benchmark._phases.begin(); it != benchmark._phases.end(); ++it)
        ChooseBestBenchmarkMetrics(**it);
}

void Launcher::ChooseBestBenchmarkMetrics(PhaseCore& phase)
{
    for (auto it = phase._child.begin(); it != phase._child.end(); ++it)
        ChooseBestBenchmarkMetrics(**it);
    phase._best = (phase._metrics.total_time() < phase._best.total_time()) ? phase._metrics : phase._best;
}

void Launcher::UpdateFinalBenchmarkMetrics(Benchmark& benchmark)
{
    for (auto it = benchmark._phases.begin(); it != benchmark._phases.end(); ++it)
        UpdateFinalBenchmarkMetrics(**it, (*it)->name());
}

void Launcher::UpdateFinalBenchmarkMetrics(PhaseCore& phase, const std::string& name)
{
    for (auto it = phase._child.begin(); it != phase._child.end(); ++it)
        UpdateFinalBenchmarkMetrics(**it, name + '.' + (*it)->name());
    phase._name = name;
    phase._metrics = phase._best;
}

} // namespace CppBenchmark
