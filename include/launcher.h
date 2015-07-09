//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#ifndef CPPBENCHMARK_LAUNCHER_H
#define CPPBENCHMARK_LAUNCHER_H

#include "benchmark.h"
#include "reporter.h"

namespace CppBenchmark {

class Launcher
{
public:
    Launcher() = default;
    Launcher(const Benchmark&) = delete;
    Launcher(Benchmark&&) = delete;
    ~Launcher() = default;

    Launcher& operator=(const Launcher&) = delete;
    Launcher& operator=(Launcher&&) = delete;

    void AddBenchmark(std::shared_ptr<Benchmark> benchmark) { _benchmarks.emplace_back(benchmark); }
    void ClearAllBenchmarks() { _benchmarks.clear(); }

    void AddReporter(std::shared_ptr<Reporter> reporter) { _reporters.emplace_back(reporter); }
    void ClearAllReporters() { _reporters.clear(); }

    void Launch(const std::string& pattern = ".",
                std::function<void (const Benchmark&, const Context&, int)> onLaunching = [](const Benchmark&, const Context&, int){},
                std::function<void (const Benchmark&, const Context&, int)> onLaunched = [](const Benchmark&, const Context&, int){});

private:
    std::vector<std::shared_ptr<Benchmark>> _benchmarks;
    std::vector<std::shared_ptr<Reporter>> _reporters;

    void ReportPhase(Reporter& reporter, const PhaseCore& phase, const std::string& name);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_LAUNCHER_H
