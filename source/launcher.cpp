//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#include "launcher.h"

#include <regex>

namespace CppBenchmark {

void Launcher::Launch(const std::string& pattern,
                      std::function<void (const Benchmark&, const Context&, int)> onLaunching,
                      std::function<void (const Benchmark&, const Context&, int)> onLaunched)
{
    // Launch all suitable benchmarks
    std::regex matcher(pattern);
    for (auto benchmark : _benchmarks)
        if (std::regex_match(benchmark->name(), matcher))
            benchmark->Launch(onLaunching, onLaunched);

    // Report system & environment information
    for (auto reporter : _reporters) {
        reporter->ReportSystemInformation();
        reporter->ReportEnvironmentInformation();
    }

    // Report benchmarks information
    for (auto benchmark : _benchmarks) {
        if (std::regex_match(benchmark->name(), matcher)) {
            for (auto reporter : _reporters) {
                reporter->ReportBenchmarkInformation(*benchmark, benchmark->_settings);
                ReportPhase(*reporter, benchmark->_root);
            }
        }
    }
}

void Launcher::ReportPhase(Reporter& reporter, const PhaseCore& phase)
{
    reporter.ReportPhaseInformation(phase, phase.metrics());
    for (auto child : phase._child)
        ReportPhase(reporter, *child.second);
}

} // namespace CppBenchmark
