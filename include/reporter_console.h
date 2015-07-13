//
// Created by Ivan Shynkarenka on 13.07.2015.
//

#ifndef CPPBENCHMARK_REPORTER_CONSOLE_H
#define CPPBENCHMARK_REPORTER_CONSOLE_H

#include "reporter.h"

#include <iostream>

namespace CppBenchmark {

class ReporterConsole : Reporter
{
public:
    ReporterConsole(std::ostream& stream = std::cout) : _stream(stream) {}
    ReporterConsole(const ReporterConsole&) = delete;
    ReporterConsole(ReporterConsole&&) = delete;
    ~ReporterConsole() = default;

    ReporterConsole& operator=(const ReporterConsole&) = delete;
    ReporterConsole& operator=(ReporterConsole&&) = delete;

    void ReportHeader() override;
    void ReportSystem() override;
    void ReportEnvironment() override;
    void ReportBenchmark(const Benchmark& benchmark, const Settings& settings) override;
    void ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics) override;
    void ReportFooter() override;

private:
    std::ostream& _stream;

    std::string ReportSeparator(char ch);
    std::string ReportSize(int64_t bytes);
    std::string ReportTime(int64_t nanoseconds);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_CONSOLE_H
