/*!
    \file reporter_csv.h
    \brief Comma-separated values (CSV) reporter definition
    \author Ivan Shynkarenka
    \date 10.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_REPORTER_CSV_H
#define CPPBENCHMARK_REPORTER_CSV_H

#include "benchmark/reporter.h"

#include <iostream>

namespace CppBenchmark {

//! Comma-separated values (CSV) reporter
/*!
    Reports benchmarks results in comma-separated values (CSV) format.

    More information about comma-separated values format you can find here: https://en.wikipedia.org/wiki/Comma-separated_values
*/
class ReporterCSV : public Reporter
{
public:
    //! Default class constructor
    /*!
        Initialize reporter with an output stream. Default output stream is std::cout.

        \param stream - Output stream
    */
    ReporterCSV(std::ostream& stream = std::cout) : _stream(stream) {}
    ReporterCSV(const ReporterCSV&) = delete;
    ReporterCSV(ReporterCSV&&) = delete;
    virtual ~ReporterCSV() = default;

    ReporterCSV& operator=(const ReporterCSV&) = delete;
    ReporterCSV& operator=(ReporterCSV&&) = delete;

    // Implementation of Reporter
    void ReportHeader() override;
    void ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics) override;

private:
    std::ostream& _stream;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_CSV_H
