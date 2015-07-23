//
// Created by Ivan Shynkarenka on 14.07.2015.
//

#ifndef CPPBENCHMARK_REPORTER_JSON_H
#define CPPBENCHMARK_REPORTER_JSON_H

#include "reporter.h"

#include <iostream>

namespace CppBenchmark {

class ReporterJSON : public Reporter
{
public:
    ReporterJSON(std::ostream& stream = std::cout, MetricsReportingType type = eReportBest) : _stream(stream), _type(type) {}
    ReporterJSON(const ReporterJSON&) = delete;
    ReporterJSON(ReporterJSON&&) = delete;
    ~ReporterJSON() = default;

    ReporterJSON& operator=(const ReporterJSON&) = delete;
    ReporterJSON& operator=(ReporterJSON&&) = delete;

    void ReportHeader() override;
    void ReportSystem() override;
    void ReportEnvironment() override;
    void ReportBenchmarksHeader() override;
    void ReportBenchmarksFooter() override;
    void ReportBenchmarkHeader() override;
    void ReportBenchmarkFooter() override;
    void ReportBenchmark(const Benchmark& benchmark, const Settings& settings) override;
    void ReportPhasesHeader() override;
    void ReportPhasesFooter() override;
    void ReportPhaseHeader() override;
    void ReportPhaseFooter() override;
    void ReportPhase(const PhaseCore& phase, const PhaseMetrics& best, const PhaseMetrics& worst) override;
    void ReportFooter() override;

private:
    std::ostream& _stream;
    MetricsReportingType _type;
    bool _benchmark_comma;
    bool _phase_comma;

    template <typename T>
    T ReportValue(T best, T worst) { return (_type == eReportBest) ? best : ((_type == eReportWorst) ? worst : ((best > worst) ? (best - worst) : (worst - best)));}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_JSON_H
