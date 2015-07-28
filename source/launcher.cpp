//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#include "launcher.h"

#include <regex>

namespace CppBenchmark {

void Launcher::Launch(const std::string& pattern)
{
    std::regex matcher(pattern);

    // For all registered benchmarks...
    for (auto benchmark : _benchmarks) {
        // Match benchmark name with the given pattern
        if (pattern.empty() || std::regex_match(benchmark->name(), matcher)) {
            // Launch benchmark
            benchmark->Launch(this);
        }
    }
}

void Launcher::Report(Reporter& reporter)
{
    // Report header, system & environment
    reporter.ReportHeader();
    reporter.ReportSystem();
    reporter.ReportEnvironment();
    reporter.ReportBenchmarksHeader();

    // For all registered benchmarks...
    for (auto benchmark : _benchmarks) {
        // Filter performed benchmarks
        if (benchmark->is_launched()) {
            // Report benchmark results
            reporter.ReportBenchmarkHeader();
            reporter.ReportBenchmark(*benchmark, benchmark->_settings);
            reporter.ReportPhasesHeader();
            for (auto root_phase : benchmark->_phases) {
                ReportPhase(reporter, *root_phase, root_phase->name());
            }
            reporter.ReportPhasesFooter();
            reporter.ReportBenchmarkFooter();
        }
    }

    // Report footer
    reporter.ReportBenchmarksFooter();
    reporter.ReportFooter();
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

} // namespace CppBenchmark
