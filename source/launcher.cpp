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

    // Report header, system & environment
    for (auto reporter : _reporters) {
        reporter->ReportHeader();
        reporter->ReportSystem();
        reporter->ReportEnvironment();
    }

    // Report benchmark results
    for (auto benchmark : _benchmarks) {
        if (std::regex_match(benchmark->name(), matcher)) {
            for (auto reporter : _reporters) {
                reporter->ReportBenchmark(benchmark->name(), benchmark->_root.metrics(), benchmark->_settings);
                ReportPhase(*reporter, benchmark->_root, benchmark->name());
            }
        }
    }

    // Report footer
    for (auto reporter : _reporters)
        reporter->ReportFooter();
}

void Launcher::ReportPhase(Reporter& reporter, const PhaseCore& phase, const std::string& name)
{
    for (auto child : phase._child) {
        std::string child_name = name + '.' + child.second->name();
        reporter.ReportPhase(child_name, child.second->metrics());
        ReportPhase(reporter, *child.second, child_name);
    }
}

} // namespace CppBenchmark
