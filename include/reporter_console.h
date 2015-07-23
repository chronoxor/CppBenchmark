//
// Created by Ivan Shynkarenka on 13.07.2015.
//

#ifndef CPPBENCHMARK_REPORTER_CONSOLE_H
#define CPPBENCHMARK_REPORTER_CONSOLE_H

#include "reporter.h"

#include <iostream>

namespace CppBenchmark {

class ReporterConsole : public Reporter
{
public:
    ReporterConsole(std::ostream& stream = std::cout, MetricsReportingType type = eReportBest) : _stream(stream), _type(type) {}
    ReporterConsole(const ReporterConsole&) = delete;
    ReporterConsole(ReporterConsole&&) = delete;
    ~ReporterConsole() = default;

    ReporterConsole& operator=(const ReporterConsole&) = delete;
    ReporterConsole& operator=(ReporterConsole&&) = delete;

    void ReportHeader() override;
    void ReportSystem() override;
    void ReportEnvironment() override;
    void ReportBenchmark(const Benchmark& benchmark, const Settings& settings) override;
    void ReportPhase(const PhaseCore& phase, const PhaseMetrics& best, const PhaseMetrics& worst) override;
    void ReportFooter() override;

    static std::string GenerateSeparator(char ch);
    static std::string GenerateClockSpeed(int64_t hertz);
    static std::string GenerateSize(int64_t bytes);
    static std::string GenerateTime(int64_t nanoseconds);

private:
    std::ostream& _stream;
    MetricsReportingType _type;

    template <typename T>
    T ReportValue(T best, T worst) { return (_type == eReportBest) ? best : ((_type == eReportWorst) ? worst : ((best > worst) ? (best - worst) : (worst - best)));}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_REPORTER_CONSOLE_H
