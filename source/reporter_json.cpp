//
// Created by Ivan Shynkarenka on 14.07.2015.
//

#include "reporter_json.h"

#include <iomanip>

#include "environment.h"
#include "system.h"
#include "version.h"

namespace CppBenchmark {

namespace Internals {

std::string indent1 = std::string(2, ' ');
std::string indent2 = std::string(4, ' ');
std::string indent3 = std::string(6, ' ');
std::string indent4 = std::string(8, ' ');
std::string indent5 = std::string(10, ' ');
std::string indent6 = std::string(12, ' ');
std::string indent7 = std::string(14, ' ');

} // namespace Internals

void ReporterJSON::ReportHeader()
{
    _stream << "{\n";
    _stream << Internals::indent1 << "\"version\": \"" << version << "\",\n";
}

void ReporterJSON::ReportSystem()
{
    _stream << Internals::indent1 << "\"system\": {\n";
    _stream << Internals::indent2 << "\"cpu_architecture\": \"" << System::CpuArchitecture() << "\",\n";
    _stream << Internals::indent2 << "\"cpu_logical_cores\": " << System::CpuLogicalCores() << ",\n";
    _stream << Internals::indent2 << "\"cpu_physical_cores\": " << System::CpuPhysicalCores() << ",\n";
    _stream << Internals::indent2 << "\"cpu_clock_speed\": " << System::CpuClockSpeed() << ",\n";
    _stream << Internals::indent2 << "\"cpu_hyper_threading\": " << (System::CpuHyperThreading() ? "true" : "false") << ",\n";
    _stream << Internals::indent2 << "\"ram_total\": " << System::RamTotal() << ",\n";
    _stream << Internals::indent2 << "\"ram_free\": " << System::RamFree() << "\n";
    _stream << Internals::indent1 << "},\n";
}

void ReporterJSON::ReportEnvironment()
{
    _stream << Internals::indent1 << "\"environment\": {\n";
    _stream << Internals::indent2 << "\"is_64_bit_os\": " << (Environment::Is64BitOS() ? "true" : "false") << ",\n";
    _stream << Internals::indent2 << "\"is_32_bit_os\": " << (Environment::Is32BitOS() ? "true" : "false") << ",\n";
    _stream << Internals::indent2 << "\"is_64_bit_process\": " << (Environment::Is64BitProcess() ? "true" : "false") << ",\n";
    _stream << Internals::indent2 << "\"is_32_bit_process\": " << (Environment::Is32BitProcess() ? "true" : "false") << ",\n";
    _stream << Internals::indent2 << "\"is_debug\": " << (Environment::IsDebug() ? "true" : "false") << ",\n";
    _stream << Internals::indent2 << "\"is_release\": " << (Environment::IsRelease() ? "true" : "false") << ",\n";
    _stream << Internals::indent2 << "\"timestamp\": " << Environment::Timestamp() << "\n";
    _stream << Internals::indent1 << "},\n";
}

void ReporterJSON::ReportBenchmarksHeader()
{
    _stream << Internals::indent1 << "\"benchmarks\": [\n";

    // Reset benchmark comma
    _benchmark_comma = false;
}

void ReporterJSON::ReportBenchmarksFooter()
{
    _stream << '\n';
    _stream << Internals::indent1 << "]\n";
}

void ReporterJSON::ReportBenchmarkHeader()
{
    if (_benchmark_comma) {
        _stream << ",\n";
    }
    _stream << Internals::indent2 << "{\n";
    _stream << Internals::indent3 << "\"benchmark\": {\n";
    _benchmark_comma = true;
}

void ReporterJSON::ReportBenchmarkFooter()
{
    _stream << Internals::indent3 <<  "}\n";
    _stream << Internals::indent2 <<  "}";
}

void ReporterJSON::ReportBenchmark(const Benchmark& benchmark, const Settings& settings)
{
    _stream << Internals::indent4 << "\"name\": \"" << benchmark.name() << "\",\n";
    _stream << Internals::indent4 << "\"metrics\": \"" << to_string(_type) << "\",\n";
    _stream << Internals::indent4 << "\"attempts\": " << settings.attempts() << ",\n";
    if (settings.iterations() > 0)
        _stream << Internals::indent4 << "\"iterations\": " << settings.iterations() << ",\n";
    if (settings.nanoseconds() > 0)
        _stream << Internals::indent4 << "\"nanoseconds\": " << settings.nanoseconds() << ",\n";
}

void ReporterJSON::ReportPhasesHeader()
{
    _stream << Internals::indent4 << "\"phases\": [\n";

    // Reset phase comma
    _phase_comma = false;
}

void ReporterJSON::ReportPhasesFooter()
{
    _stream << '\n';
    _stream << Internals::indent4 << "]\n";
}

void ReporterJSON::ReportPhaseHeader()
{
    if (_phase_comma) {
        _stream << ",\n";
    }
    _stream << Internals::indent5 << "{\n";
    _stream << Internals::indent6 << "\"phase\": {\n";
    _phase_comma = true;
}

void ReporterJSON::ReportPhaseFooter()
{
    _stream << Internals::indent6 << "}\n";
    _stream << Internals::indent5 << "}";
}

void ReporterJSON::ReportPhase(const PhaseCore& phase, const PhaseMetrics& best, const PhaseMetrics& worst)
{
    _stream << Internals::indent7 << "\"name\": \"" << phase.name() << "\",\n";
    _stream << Internals::indent7 << "\"avg_time\": " << ReportValue(best.avg_time(), worst.avg_time()) << ",\n";
    _stream << Internals::indent7 << "\"min_time\": " << ReportValue(best.min_time(), worst.min_time()) << ",\n";
    _stream << Internals::indent7 << "\"max_time\": " << ReportValue(best.max_time(), worst.max_time()) << ",\n";
    _stream << Internals::indent7 << "\"total_time\": " << ReportValue(best.total_time(), worst.total_time()) << ",\n";
    if ((best.total_iterations() > 1) || (worst.total_iterations() > 1))
        _stream << Internals::indent7 << "\"total_iterations\": " << ReportValue(best.total_iterations(), worst.total_iterations()) << ",\n";
    if ((best.total_items() > 0) || (worst.total_items() > 0))
        _stream << Internals::indent7 << "\"total_items\": " << ReportValue(best.total_items(), worst.total_items()) << ",\n";
    if ((best.total_bytes() > 0) || (worst.total_bytes() > 0))
        _stream << Internals::indent7 << "\"total_bytes\": " << ReportValue(best.total_bytes(), worst.total_bytes()) << ",\n";
    if ((best.total_iterations() > 1) || (worst.total_iterations() > 1))
        _stream << Internals::indent7 << std::fixed << std::setprecision(3) << "\"iterations_per_second\": " << ReportValue(best.iterations_per_second(), worst.iterations_per_second()) << ",\n";
    if ((best.total_items() > 0) || (worst.total_items() > 0))
        _stream << Internals::indent7 << std::fixed << std::setprecision(3) << "\"items_per_second\": " << ReportValue(best.items_per_second(), worst.items_per_second()) << ",\n";
    if ((best.total_bytes() > 0) || (worst.total_bytes() > 0))
        _stream << Internals::indent7 << std::fixed << std::setprecision(3) << "\"bytes_per_second\": " << ReportValue(best.bytes_per_second(), worst.bytes_per_second()) << "\n";
}

void ReporterJSON::ReportFooter()
{
    _stream << "}\n";
}

} // namespace CppBenchmark
