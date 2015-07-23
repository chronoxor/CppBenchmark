//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#ifndef CPPBENCHMARK_LAUNCHER_H
#define CPPBENCHMARK_LAUNCHER_H

#include "benchmark.h"
#include "benchmark_threads.h"
#include "launcher_handler.h"
#include "reporter.h"

namespace CppBenchmark {

class Launcher : public LauncherHandler
{
public:
    Launcher() = default;
    Launcher(const Benchmark&) = delete;
    Launcher(Benchmark&&) = delete;
    virtual ~Launcher() = default;

    Launcher& operator=(const Launcher&) = delete;
    Launcher& operator=(Launcher&&) = delete;

    void AddBenchmark(std::shared_ptr<Benchmark> benchmark) { _benchmarks.emplace_back(benchmark); }
    void ClearAllBenchmarks() { _benchmarks.clear(); }

    virtual void Launch(const std::string& pattern = "");

    void Report(Reporter& reporter);

protected:
    std::vector<std::shared_ptr<Benchmark>> _benchmarks;

private:
    void ReportPhase(Reporter& reporter, const PhaseCore& phase, const std::string& name);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_LAUNCHER_H
