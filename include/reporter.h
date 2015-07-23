//
// Created by Ivan Shynkarenka on 08.07.2015.
//

#ifndef CPPBENCHMARK_REPORTER_H
#define CPPBENCHMARK_REPORTER_H

#include "benchmark.h"

namespace CppBenchmark {

enum MetricsReportingType
{
    eReportBest,
    eReportWorst,
    eReportDiff
};

std::string to_string(MetricsReportingType value);

class Reporter
{
public:
    Reporter() = default;
    Reporter(const Reporter&) = delete;
    Reporter(Reporter&&) = delete;
    ~Reporter() = default;

    Reporter& operator=(const Reporter&) = delete;
    Reporter& operator=(Reporter&&) = delete;

    virtual void ReportHeader() {}
    virtual void ReportSystem() {}
    virtual void ReportEnvironment() {}
    virtual void ReportBenchmarksHeader() {}
    virtual void ReportBenchmarksFooter() {}
    virtual void ReportBenchmarkHeader() {}
    virtual void ReportBenchmarkFooter() {}
    virtual void ReportBenchmark(const Benchmark&, const Settings&) {}
    virtual void ReportPhasesHeader() {}
    virtual void ReportPhasesFooter() {}
    virtual void ReportPhaseHeader() {}
    virtual void ReportPhaseFooter() {}
    virtual void ReportPhase(const PhaseCore& phase, const PhaseMetrics& best, const PhaseMetrics& worst) = 0;
    virtual void ReportFooter() {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_H
