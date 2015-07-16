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
    virtual ~Launcher() = default;

    Launcher& operator=(const Launcher&) = delete;
    Launcher& operator=(Launcher&&) = delete;

    void AddBenchmark(std::shared_ptr<Benchmark> benchmark) { _benchmarks.emplace_back(benchmark); }
    void ClearAllBenchmarks() { _benchmarks.clear(); }

    void AddReporter(std::shared_ptr<Reporter> reporter) { _reporters.emplace_back(reporter); }
    void ClearAllReporters() { _reporters.clear(); }

    virtual void Launch() { LaunchWithFilter(""); }

protected:
    std::vector<std::shared_ptr<Benchmark>> _benchmarks;
    std::vector<std::shared_ptr<Reporter>> _reporters;

    virtual void onLaunching(const Benchmark& benchmark, const Context& context, int attempt) {}
    virtual void onLaunched(const Benchmark& benchmark, const Context& context, int attempt) {}

    void MatchWithFilter(const std::string& pattern, std::function<void (Benchmark& benchmark)> action);
    void LaunchWithFilter(const std::string& pattern);

private:
    void LaunchBenchmark(Benchmark& benchmark);

    void ReportPhase(Reporter& reporter, const PhaseCore& phase, const std::string& name);

    static void ResetBenchmark(Benchmark& benchmark);
    static void UpdateBenchmark(Benchmark& benchmark, Context& context);

    static void ResetBenchmarkMetrics(Benchmark& benchmark);
    static void ResetBenchmarkMetrics(PhaseCore& phase);

    static void ChooseBestBenchmarkMetrics(Benchmark& benchmark);
    static void ChooseBestBenchmarkMetrics(PhaseCore& phase);

    static void UpdateFinalBenchmarkMetrics(Benchmark& benchmark);
    static void UpdateFinalBenchmarkMetrics(PhaseCore& phase, const std::string& name);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_LAUNCHER_H
