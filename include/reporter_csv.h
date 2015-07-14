//
// Created by Ivan Shynkarenka on 10.07.2015.
//

#ifndef CPPBENCHMARK_REPORTER_CSV_H
#define CPPBENCHMARK_REPORTER_CSV_H

#include "reporter.h"

#include <iostream>

namespace CppBenchmark {

class ReporterCSV : Reporter
{
public:
    ReporterCSV(std::ostream& stream = std::cout) : _stream(stream) {}
    ReporterCSV(const ReporterCSV&) = delete;
    ReporterCSV(ReporterCSV&&) = delete;
    ~ReporterCSV() = default;

    ReporterCSV& operator=(const ReporterCSV&) = delete;
    ReporterCSV& operator=(ReporterCSV&&) = delete;

    void ReportHeader() override;
    void ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics) override;

private:
    std::ostream& _stream;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_CSV_H
