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

    virtual void ReportSystemInformation() = 0;
    virtual void ReportEnvironmentInformation() = 0;
    virtual void ReportBenchmarkInformation(const Benchmark& benchmark, const Settings& settings) = 0;
    virtual void ReportPhaseInformation(const Phase& phase, const PhaseMetrics& metrics) = 0;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_H
