/*!
    \file reporter_json.h
    \brief JSON reporter definition
    \author Ivan Shynkarenka
    \date 14.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_REPORTER_JSON_H
#define CPPBENCHMARK_REPORTER_JSON_H

#include "benchmark/reporter.h"

#include <iostream>

namespace CppBenchmark {

//! JSON reporter
/*!
    Reports benchmarks results in JSON format.

    More information about JSON format you can find here: https://en.wikipedia.org/wiki/Comma-separated_values
*/
class ReporterJSON : public Reporter
{
public:
    //! Default class constructor
    /*!
        Initialize reporter with an output stream. Default output stream is std::cout.

        \param stream - Output stream
    */
    ReporterJSON(std::ostream& stream = std::cout) : _stream(stream), _benchmark_comma(false), _phase_comma(false) {}
    ReporterJSON(const ReporterJSON&) = delete;
    ReporterJSON(ReporterJSON&&) = delete;
    virtual ~ReporterJSON() = default;

    ReporterJSON& operator=(const ReporterJSON&) = delete;
    ReporterJSON& operator=(ReporterJSON&&) = delete;

    // Implementation of Reporter
    void ReportHeader() override;
    void ReportSystem() override;
    void ReportEnvironment() override;
    void ReportBenchmarksHeader() override;
    void ReportBenchmarksFooter() override;
    void ReportBenchmarkHeader() override;
    void ReportBenchmarkFooter() override;
    void ReportBenchmark(const BenchmarkBase& benchmark, const Settings& settings) override;
    void ReportPhasesHeader() override;
    void ReportPhasesFooter() override;
    void ReportPhaseHeader() override;
    void ReportPhaseFooter() override;
    void ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics) override;
    void ReportFooter() override;

private:
    std::ostream& _stream;
    bool _benchmark_comma;
    bool _phase_comma;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_JSON_H
