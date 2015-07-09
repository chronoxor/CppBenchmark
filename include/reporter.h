//
// Created by Ivan Shynkarenka on 08.07.2015.
//

#ifndef CPPBENCHMARK_REPORTER_H
#define CPPBENCHMARK_REPORTER_H

#include "benchmark.h"

namespace CppBenchmark {

class Reporter
{
public:
    Reporter() = default;
    Reporter(const Reporter&) = delete;
    Reporter(Reporter&&) = delete;
    ~Reporter() = default;

    Reporter& operator=(const Reporter&) = delete;
    Reporter& operator=(Reporter&&) = delete;

    virtual void ReportHeader() = 0;
    virtual void ReportSystem() = 0;
    virtual void ReportEnvironment() = 0;
    virtual void ReportBenchmark(const std::string& name, const PhaseMetrics& metrics, const Settings& settings) = 0;
    virtual void ReportPhase(const std::string& name, const PhaseMetrics& metrics) = 0;
    virtual void ReportFooter() = 0;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_H
