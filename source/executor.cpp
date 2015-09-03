/*!
    \file executor.cpp
    \brief Dynamic benchmarks executor class implementation
    \author Ivan Shynkarenka
    \date 02.09.2015
    \copyright MIT License
*/

#include "executor.h"

#include <algorithm>

namespace CppBenchmark {

//! @cond
namespace Internals {

//! Dynamic benchmark wrapper class
/*!
    Provides interface to wrap dynamic benchmark.
*/
class DynamicBenchmark : public Benchmark
{
public:
    //! Dynamic benchmark settings type
    typedef Settings TSettings;

    //! Default class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    explicit DynamicBenchmark(const std::string& name, const TSettings& settings = TSettings())
            : Benchmark(name, settings)
    {}
    DynamicBenchmark(const DynamicBenchmark&) = delete;
    DynamicBenchmark(DynamicBenchmark&&) = delete;
    virtual ~DynamicBenchmark() = default;

    DynamicBenchmark& operator=(const DynamicBenchmark&) = delete;
    DynamicBenchmark& operator=(DynamicBenchmark&&) = delete;

protected:
    // Implementation of Benchmark
    void Run(Context& context) override {}
};

} // namespace Internals
//! @endcond

std::shared_ptr<Phase> Executor::StartBenchmark(const std::string& benchmark)
{
    std::shared_ptr<PhaseCore> result;

    // Update dynamic benchmarks collection under lock guard...
    {
        std::lock_guard<std::mutex> lock(_mutex);

        // Find or create a dynamic benchmark with the given name
        auto it = std::find_if(_benchmarks.begin(), _benchmarks.end(), [&benchmark](std::shared_ptr<PhaseCore>& item) {
            return ((item->name() == benchmark) && (item->_thread == System::CurrentThreadId()));
        });
        if (it == _benchmarks.end()) {
            result = std::make_shared<PhaseCore>(benchmark);
            _benchmarks.emplace_back(result);
        }
        else
            result = *it;
    }

    // Start new iteration for the child dynamic benchmark
    result->StartCollectingMetrics();

    // Add new metrics iteration
    result->_metrics_current.AddIterations(1);

    return result;
}

void Executor::StopBenchmark(const std::string& benchmark)
{
    // Update dynamic benchmarks collection under lock guard...
    {
        std::lock_guard<std::mutex> lock(_mutex);

        // Find dynamic benchmark with the given name
        auto it = std::find_if(_benchmarks.begin(), _benchmarks.end(), [&benchmark](std::shared_ptr<PhaseCore>& item) {
            return ((item->name() == benchmark) && (item->_thread == System::CurrentThreadId()));
        });
        if (it != _benchmarks.end())
            (*it)->StopCollectingMetrics();
    }
}

void Executor::Report(Reporter& reporter)
{
    std::lock_guard<std::mutex> lock(_mutex);

    Benchmark::UpdateBenchmarkMetrics(_benchmarks);
    Benchmark::UpdateBenchmarkThreads(_benchmarks);

    // Report header, system & environment
    reporter.ReportHeader();
    reporter.ReportSystem();
    reporter.ReportEnvironment();
    reporter.ReportBenchmarksHeader();

    // For all registered benchmarks...
    for (auto benchmark : _benchmarks) {
        // Create dynamic benchmark wrapper
        Internals::DynamicBenchmark result(benchmark->name(), Settings().Attempts(1));

        // Report benchmark results
        reporter.ReportBenchmarkHeader();
        reporter.ReportBenchmark(result, result.settings());
        reporter.ReportPhasesHeader();
        ReportPhase(reporter, *benchmark, benchmark->name());
        reporter.ReportPhasesFooter();
        reporter.ReportBenchmarkFooter();
    }

    // Report footer
    reporter.ReportBenchmarksFooter();
    reporter.ReportFooter();
}

void Executor::ReportPhase(Reporter& reporter, const PhaseCore& phase, const std::string& name)
{
    reporter.ReportPhaseHeader();
    reporter.ReportPhase(phase, phase.metrics());
    reporter.ReportPhaseFooter();
    for (auto child : phase._child) {
        std::string child_name = name + "." + child->name();
        ReportPhase(reporter, *child, child_name);
    }
}

} // namespace CppBenchmark
