/*!
    \file launcher.cpp
    \brief Launcher base implementation
    \author Ivan Shynkarenka
    \date 07.07.2015
    \copyright MIT License
*/

#include "benchmark/launcher.h"

#include <regex>

namespace CppBenchmark {

void Launcher::Launch(const std::string& pattern)
{
    int current = 0;
    int total = 0;
    std::vector<std::shared_ptr<BenchmarkBase>> benchmarks;

    // Filter benchmarks
    std::regex matcher(pattern);
    for (auto& benchmark : _benchmarks)
    {
        // Match benchmark name with the given pattern
        if (pattern.empty() || std::regex_match(benchmark->name(), matcher))
        {
            total += benchmark->CountLaunches();
            benchmarks.push_back(benchmark);
        }
    }

    // Launch filtered benchmarks
    for (auto& benchmark : benchmarks)
        benchmark->Launch(current, total, *this);
}

void Launcher::Report(Reporter& reporter) const
{
    // Report header, system & environment
    reporter.ReportHeader();
    reporter.ReportSystem();
    reporter.ReportEnvironment();
    reporter.ReportBenchmarksHeader();

    // For all registered benchmarks...
    for (auto& benchmark : _benchmarks)
    {
        // Filter performed benchmarks
        if (benchmark->_launched)
        {
            // Report benchmark results
            reporter.ReportBenchmarkHeader();
            reporter.ReportBenchmark(*benchmark, benchmark->settings());
            reporter.ReportPhasesHeader();
            for (auto& root_phase : benchmark->_phases)
                ReportPhase(reporter, *root_phase, root_phase->name());
            reporter.ReportPhasesFooter();
            reporter.ReportBenchmarkFooter();
        }
    }

    // Report footer
    reporter.ReportBenchmarksFooter();
    reporter.ReportFooter();
}

void Launcher::ReportPhase(Reporter& reporter, const PhaseCore& phase, const std::string& name) const
{
    reporter.ReportPhaseHeader();
    reporter.ReportPhase(phase, phase.metrics());
    reporter.ReportPhaseFooter();
    for (auto& child : phase._child)
    {
        std::string child_name = name + "." + child->name();
        ReportPhase(reporter, *child, child_name);
    }
}

} // namespace CppBenchmark
