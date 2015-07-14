//
// Created by Ivan Shynkarenka on 14.07.2015.
//

#include "reporter_json.h"

#include "environment.h"
#include "system.h"
#include "version.h"

namespace CppBenchmark {

void ReporterJSON::ReportHeader()
{
    _stream << "{\n";
    _stream << _indent << "\"version\": \"" << version << "\",\n";
}

void ReporterJSON::ReportSystem()
{
    _stream << _indent << "\"system\": {\n";
    _stream << _indent << _indent << "\"cpu_architecture\": \"" << System::CpuArchitecture() << "\",\n";
    _stream << _indent << _indent << "\"cpu_logical_cores\": " << System::CpuLogicalCores() << ",\n";
    _stream << _indent << _indent << "\"cpu_physical_cores\": " << System::CpuPhysicalCores() << ",\n";
    _stream << _indent << _indent << "\"cpu_clock_speed\": " << System::CpuClockSpeed() << ",\n";
    _stream << _indent << _indent << "\"cpu_hyper_threading\": " << (System::CpuHyperThreading() ? "true" : "false") << ",\n";
    _stream << _indent << _indent << "\"ram_total\": " << System::RamTotal() << ",\n";
    _stream << _indent << _indent << "\"ram_free\": " << System::RamFree() << "\n";
    _stream << _indent << "},\n";
}

void ReporterJSON::ReportEnvironment()
{
    _stream << _indent << "\"environment\": {\n";
    _stream << _indent << _indent << "\"is_64_bit_os\": " << (Environment::Is64BitOS() ? "true" : "false") << ",\n";
    _stream << _indent << _indent << "\"is_32_bit_os\": " << (Environment::Is32BitOS() ? "true" : "false") << ",\n";
    _stream << _indent << _indent << "\"is_64_bit_process\": " << (Environment::Is64BitProcess() ? "true" : "false") << ",\n";
    _stream << _indent << _indent << "\"is_32_bit_process\": " << (Environment::Is32BitProcess() ? "true" : "false") << ",\n";
    _stream << _indent << _indent << "\"is_debug\": " << (Environment::IsDebug() ? "true" : "false") << ",\n";
    _stream << _indent << _indent << "\"is_release\": " << (Environment::IsRelease() ? "true" : "false") << ",\n";
    _stream << _indent << _indent << "\"timestamp\": " << Environment::Timestamp() << ",\n";
    _stream << _indent << "},\n";
}

void ReporterJSON::ReportBenchmarksHeader()
{
    _stream << _indent << "\"benchmarks\": [\n";

    // Reset benchmark comma
    _benchmark_comma = false;
}

void ReporterJSON::ReportBenchmarksFooter()
{
    _stream << '\n';
    _stream << _indent << "]\n";
}

void ReporterJSON::ReportBenchmarkHeader()
{
    if (_benchmark_comma) {
        _stream << ",\n";
    }
    _stream << _indent << _indent << "\"benchmark\": {\n";
    _benchmark_comma = true;
}

void ReporterJSON::ReportBenchmarkFooter()
{
    _stream << _indent << _indent << "}";
}

void ReporterJSON::ReportBenchmark(const Benchmark& benchmark, const Settings& settings)
{
    _stream << _indent << _indent << _indent << "\"name\": \"" << benchmark.name() << "\",\n";
    _stream << _indent << _indent << _indent << "\"attempts\": " << settings.attempts() << ",\n";
    if (settings.iterations() > 0)
        _stream << _indent << _indent << _indent << "\"iterations\": " << settings.iterations() << ",\n";
    if (settings.nanoseconds() > 0)
        _stream << _indent << _indent << _indent << "\"nanoseconds\": " << settings.nanoseconds() << ",\n";
}

void ReporterJSON::ReportPhasesHeader()
{
    _stream << _indent << _indent << _indent << "\"phases\": [\n";

    // Reset phase comma
    _phase_comma = false;
}

void ReporterJSON::ReportPhasesFooter()
{
    _stream << '\n';
    _stream << _indent << _indent << _indent << "]\n";
}

void ReporterJSON::ReportPhaseHeader()
{
    if (_phase_comma) {
        _stream << ",\n";
    }
    _stream << _indent << _indent << _indent << _indent << "\"phase\": {\n";
    _phase_comma = true;
}

void ReporterJSON::ReportPhaseFooter()
{
    _stream << _indent << _indent << _indent << _indent << "}";
}

void ReporterJSON::ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics)
{
    _stream << _indent << _indent << _indent << _indent << _indent << "\"name\": \"" << phase.name() << "\",\n";
    _stream << _indent << _indent << _indent << _indent << _indent << "\"avg_time\": \"" << metrics.avg_time() << "\",\n";
    _stream << _indent << _indent << _indent << _indent << _indent << "\"min_time\": \"" << metrics.min_time() << "\",\n";
    _stream << _indent << _indent << _indent << _indent << _indent << "\"max_time\": \"" << metrics.max_time() << "\",\n";
    _stream << _indent << _indent << _indent << _indent << _indent << "\"total_time\": \"" << metrics.total_time() << "\",\n";
    _stream << _indent << _indent << _indent << _indent << _indent << "\"total_iterations\": \"" << metrics.total_iterations() << "\",\n";
    _stream << _indent << _indent << _indent << _indent << _indent << "\"total_items\": \"" << metrics.total_items() << "\",\n";
    _stream << _indent << _indent << _indent << _indent << _indent << "\"total_bytes\": \"" << metrics.total_bytes() << "\",\n";
    _stream << _indent << _indent << _indent << _indent << _indent << "\"iterations_per_second\": \"" << metrics.iterations_per_second() << "\",\n";
    _stream << _indent << _indent << _indent << _indent << _indent << "\"items_per_second\": \"" << metrics.items_per_second() << "\",\n";
    _stream << _indent << _indent << _indent << _indent << _indent << "\"bytes_per_second\": \"" << metrics.bytes_per_second() << "\",\n";
}

void ReporterJSON::ReportFooter()
{
    _stream << "}\n";
}

} // namespace CppBenchmark
