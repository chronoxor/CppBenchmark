//
// Created by Ivan Shynkarenka on 14.07.2015.
//

#include "reporter_json.h"

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
    _stream << Internals::indent4 << "\"attempts\": " << settings.attempts() << ",\n";
    if (settings.iterations() > 0)
        _stream << Internals::indent4 << "\"iterations\": " << settings.iterations() << ",\n";
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

void ReporterJSON::ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics)
    {
    _stream << Internals::indent7 << "\"name\": \"" << phase.name() << "\",\n";
    if (metrics.total_iterations() > 0) {
        _stream << Internals::indent7 << "\"avg_time\": " << metrics.avg_time() << ",\n";
        _stream << Internals::indent7 << "\"min_time\": " << metrics.min_time() << ",\n";
        _stream << Internals::indent7 << "\"max_time\": " << metrics.max_time() << ",\n";
    }
    _stream << Internals::indent7 << "\"total_time\": " << metrics.total_time() << ",\n";
    if (metrics.total_iterations() > 0)
        _stream << Internals::indent7 << "\"total_iterations\": " << metrics.total_iterations() << ",\n";
    if (metrics.total_items() > 0)
        _stream << Internals::indent7 << "\"total_items\": " << metrics.total_items() << ",\n";
    if (metrics.total_bytes() > 0)
        _stream << Internals::indent7 << "\"total_bytes\": " << metrics.total_bytes() << ",\n";
    if (metrics.total_iterations() > 0)
        _stream << Internals::indent7 << "\"iterations_per_second\": " << metrics.iterations_per_second() << ",\n";
    if (metrics.total_items() > 0)
        _stream << Internals::indent7 << "\"items_per_second\": " << metrics.items_per_second() << ",\n";
    if (metrics.total_bytes() > 0)
        _stream << Internals::indent7 << "\"bytes_per_second\": " << metrics.bytes_per_second() << "\n";
}

void ReporterJSON::ReportFooter()
{
    _stream << "}\n";
}

} // namespace CppBenchmark
