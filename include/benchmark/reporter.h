/*!
    \file reporter.h
    \brief Reporter base definition
    \author Ivan Shynkarenka
    \date 08.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_REPORTER_H
#define CPPBENCHMARK_REPORTER_H

#include "benchmark/benchmark_base.h"

namespace CppBenchmark {

//! Reporter base class
/*!
    Provides interface to report benchmark results.
*/
class Reporter
{
public:
    Reporter() = default;
    Reporter(const Reporter&) = delete;
    Reporter(Reporter&&) = delete;
    virtual ~Reporter() = default;

    Reporter& operator=(const Reporter&) = delete;
    Reporter& operator=(Reporter&&) = delete;

    //! Report header
    virtual void ReportHeader() {}
    //! Report system information
    virtual void ReportSystem() {}
    //! Report environment information
    virtual void ReportEnvironment() {}
    //! Report all benchmarks header
    virtual void ReportBenchmarksHeader() {}
    //! Report all benchmarks footer
    virtual void ReportBenchmarksFooter() {}
    //! Report current benchmark header
    virtual void ReportBenchmarkHeader() {}
    //! Report current benchmark footer
    virtual void ReportBenchmarkFooter() {}
    //! Report current benchmark information
    /*!
        \param benchmark - Current benchmark
        \param settings - Current benchmark settings
    */
    virtual void ReportBenchmark(const BenchmarkBase& benchmark, const Settings& settings) {}
    //! Report all phases header
    virtual void ReportPhasesHeader() {}
    //! Report all phases footer
    virtual void ReportPhasesFooter() {}
    //! Report current phase header
    virtual void ReportPhaseHeader() {}
    //! Report current phase footer
    virtual void ReportPhaseFooter() {}
    //! Report current phase information
    /*!
        \param phase - Current phase
        \param metrics - Current phase metrics
    */
    virtual void ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics) = 0;
    //! Report footer
    virtual void ReportFooter() {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_H
