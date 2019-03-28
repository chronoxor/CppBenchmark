/*!
    \file launcher.cpp
    \brief Launcher base implementation
    \author Ivan Shynkarenka
    \date 07.07.2015
    \copyright MIT License
*/

#include "benchmark/launcher.h"

#include <algorithm>
#include <regex>

namespace CppBenchmark {

void Launcher::Launch(const std::string& pattern)
{
    int current = 0;
    int total = 0;
    std::vector<std::shared_ptr<BenchmarkBase>> benchmarks;

    // Build pending benchmark
    for (const auto& builder : _builders)
        AddBenchmark(builder());

    // Filter benchmarks
    std::regex matcher(pattern);
    for (const auto& benchmark : _benchmarks)
    {
        // Match benchmark name with the given pattern
        if (pattern.empty() || std::regex_match(benchmark->name(), matcher))
        {
            total += benchmark->CountLaunches();
            benchmarks.push_back(benchmark);
        }
    }

    // Launch filtered benchmarks
    for (const auto& benchmark : benchmarks)
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
    for (const auto& benchmark : _benchmarks)
    {
        // Filter performed benchmarks
        if (benchmark->_launched)
        {
            // Report benchmark results
            reporter.ReportBenchmarkHeader();
            reporter.ReportBenchmark(*benchmark, benchmark->settings());
            reporter.ReportPhasesHeader();
            for (const auto& root_phase : benchmark->_phases)
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
    for (const auto& child : phase._child)
    {
        std::string child_name = name + "." + child->name();
        ReportPhase(reporter, *child, child_name);
    }
}

void Launcher::ReportHistograms(int32_t resolution) const
{
    // For all registered benchmarks...
    for (const auto& benchmark : _benchmarks)
    {
        // Filter performed benchmarks
        if (benchmark->_launched)
        {
            // Report benchmark histograms
            for (const auto& root_phase : benchmark->_phases)
                ReportPhaseHistograms(resolution, *root_phase, root_phase->name());
        }
    }
}

void Launcher::ReportPhaseHistograms(int32_t resolution, const PhaseCore& phase, const std::string& name) const
{
    ReportPhaseHistogram(resolution, phase, name);
    for (const auto& child : phase._child)
    {
        std::string child_name = name + "." + child->name();
        ReportPhaseHistograms(resolution, *child, child_name);
    }
}

void Launcher::ReportPhaseHistogram(int32_t resolution, const PhaseCore& phase, const std::string& name) const
{
    if (phase.metrics().latency())
    {
        const char deprecated[] = "\\/?%*:|\"<>";

        // Validate filename
        std::string filename(name + ".hdr");
        for (auto ch : filename)
            if ((ch != '\\') && (ch != '/') && (std::find(deprecated, deprecated + sizeof(deprecated), ch) != (deprecated + sizeof(deprecated))))
                ch = '_';

        // Open histogram filename
        FILE* file = fopen(filename.c_str(), "w");
        if (file != nullptr)
        {
            // Print histogram
            phase.PrintLatencyHistogram(file, resolution);

            // Close file
            fclose(file);
        }
    }
}

} // namespace CppBenchmark
