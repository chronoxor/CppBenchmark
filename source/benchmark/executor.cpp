/*!
    \file executor.cpp
    \brief Dynamic benchmarks executor implementation
    \author Ivan Shynkarenka
    \date 02.09.2015
    \copyright MIT License
*/

#include "benchmark/executor.h"

#include <algorithm>

namespace CppBenchmark {

//! @cond INTERNALS
namespace Internals {

//! Dynamic benchmark wrapper class
/*!
    Provides interface to wrap dynamic benchmark.
*/
class DynamicBenchmark : public BenchmarkBase
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
        : BenchmarkBase(name, settings)
    {}
    DynamicBenchmark(const DynamicBenchmark&) = delete;
    DynamicBenchmark(DynamicBenchmark&&) = delete;
    virtual ~DynamicBenchmark() = default;

    DynamicBenchmark& operator=(const DynamicBenchmark&) = delete;
    DynamicBenchmark& operator=(DynamicBenchmark&&) = delete;
};

} // namespace Internals
//! @endcond

std::shared_ptr<Phase> Executor::StartBenchmark(const std::string& benchmark)
{
    Executor& instance = GetInstance();

    std::shared_ptr<PhaseCore> result;

    // Update dynamic benchmarks collection under lock guard...
    {
        std::scoped_lock lock(instance._mutex);

        // Find or create a dynamic benchmark with the given name
        auto it = std::find_if(instance._benchmarks.begin(), instance._benchmarks.end(), [&benchmark](const std::shared_ptr<PhaseCore>& item)
        {
            return ((item->name() == benchmark) && (item->_thread == System::CurrentThreadId()));
        });
        if (it == instance._benchmarks.end())
        {
            result = std::make_shared<PhaseCore>(benchmark);
            instance._benchmarks.emplace_back(result);
        }
        else
            result = *it;
    }

    // Start new operation for the child dynamic benchmark
    result->StartCollectingMetrics();

    // Add new metrics operation
    result->_metrics_current.AddOperations(1);

    return result;
}

void Executor::StopBenchmark(const std::string& benchmark)
{
    Executor& instance = GetInstance();

    // Update dynamic benchmarks collection under lock guard...
    {
        std::scoped_lock lock(instance._mutex);

        // Find dynamic benchmark with the given name
        auto it = std::find_if(instance._benchmarks.begin(), instance._benchmarks.end(), [&benchmark](const std::shared_ptr<PhaseCore>& item)
        {
            return ((item->name() == benchmark) && (item->_thread == System::CurrentThreadId()));
        });
        if (it != instance._benchmarks.end())
            (*it)->StopCollectingMetrics();
    }
}

void Executor::Report(Reporter& reporter)
{
    Executor& instance = GetInstance();

    std::scoped_lock lock(instance._mutex);

    BenchmarkBase::UpdateBenchmarkMetrics(instance._benchmarks);
    BenchmarkBase::UpdateBenchmarkThreads(instance._benchmarks);

    // Report header, system & environment
    reporter.ReportHeader();
    reporter.ReportSystem();
    reporter.ReportEnvironment();
    reporter.ReportBenchmarksHeader();

    // For all registered benchmarks...
    for (const auto& benchmark : instance._benchmarks)
    {
        // Create dynamic benchmark wrapper
        Internals::DynamicBenchmark result(benchmark->name(), Settings().Attempts(1));

        // Report benchmark results
        reporter.ReportBenchmarkHeader();
        reporter.ReportBenchmark(result, result.settings());
        reporter.ReportPhasesHeader();
        instance.ReportPhase(reporter, *benchmark, benchmark->name());
        reporter.ReportPhasesFooter();
        reporter.ReportBenchmarkFooter();
    }

    // Report footer
    reporter.ReportBenchmarksFooter();
    reporter.ReportFooter();
}

void Executor::ReportPhase(Reporter& reporter, const PhaseCore& phase, const std::string& name) const
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

} // namespace CppBenchmark
