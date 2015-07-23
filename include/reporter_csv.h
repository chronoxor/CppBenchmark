//
// Created by Ivan Shynkarenka on 10.07.2015.
//

#ifndef CPPBENCHMARK_REPORTER_CSV_H
#define CPPBENCHMARK_REPORTER_CSV_H

#include "reporter.h"

#include <iostream>

namespace CppBenchmark {

class ReporterCSV : public Reporter
{
public:
    ReporterCSV(std::ostream& stream = std::cout, MetricsReportingType type = eReportBest) : _stream(stream), _type(type) {}
    ReporterCSV(const ReporterCSV&) = delete;
    ReporterCSV(ReporterCSV&&) = delete;
    ~ReporterCSV() = default;

    ReporterCSV& operator=(const ReporterCSV&) = delete;
    ReporterCSV& operator=(ReporterCSV&&) = delete;

    void ReportHeader() override;
    void ReportPhase(const PhaseCore& phase, const PhaseMetrics& best, const PhaseMetrics& worst) override;

private:
    std::ostream& _stream;
    MetricsReportingType _type;

    template <typename T>
    T ReportValue(T best, T worst) { return (_type == eReportBest) ? best : ((_type == eReportWorst) ? worst : ((best > worst) ? (best - worst) : (worst - best)));}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_CSV_H
