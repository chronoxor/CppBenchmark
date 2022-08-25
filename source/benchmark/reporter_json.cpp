/*!
    \file reporter_json.cpp
    \brief JSON reporter implementation
    \author Ivan Shynkarenka
    \date 14.07.2015
    \copyright MIT License
*/

#include "benchmark/reporter_json.h"

#include "benchmark/environment.h"
#include "benchmark/version.h"

#include <set>

namespace CppBenchmark {

//! @cond INTERNALS
namespace Internals {

std::string indent1 = std::string(2, ' ');
std::string indent2 = std::string(4, ' ');
std::string indent3 = std::string(6, ' ');
std::string indent4 = std::string(8, ' ');
std::string indent5 = std::string(10, ' ');
std::string indent6 = std::string(12, ' ');
std::string indent7 = std::string(14, ' ');
std::string indent8 = std::string(16, ' ');

} // namespace Internals
//! @endcond

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
    _stream << Internals::indent2 << "\"os_version\": \"" << Environment::OSVersion() << "\",\n";
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
    if (_benchmark_comma)
        _stream << ",\n";
    _stream << Internals::indent2 << "{\n";
    _stream << Internals::indent3 << "\"benchmark\": {\n";
    _benchmark_comma = true;
}

void ReporterJSON::ReportBenchmarkFooter()
{
    _stream << Internals::indent3 <<  "}\n";
    _stream << Internals::indent2 <<  "}";
}

void ReporterJSON::ReportBenchmark(const BenchmarkBase& benchmark, const Settings& settings)
{
    _stream << Internals::indent4 << "\"name\": \"" << benchmark.name() << "\",\n";
    _stream << Internals::indent4 << "\"attempts\": " << settings.attempts() << ",\n";
    if (settings.duration() > 0)
        _stream << Internals::indent4 << "\"duration\": " << settings.duration() << ",\n";
    if (settings.operations() > 0)
        _stream << Internals::indent4 << "\"operations\": " << settings.operations() << ",\n";
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
    if (_phase_comma)
        _stream << ",\n";
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
    if (metrics.total_operations() > 1)
    {
        if (metrics.latency())
        {
            _stream << Internals::indent7 << "\"min_latency\": " << metrics.min_latency() << ",\n";
            _stream << Internals::indent7 << "\"max_latency\": " << metrics.max_latency() << ",\n";
            _stream << Internals::indent7 << "\"mean_latency\": " << metrics.mean_latency() << ",\n";
            _stream << Internals::indent7 << "\"stdv_latency\": " << metrics.stdv_latency() << ",\n";
        }
        else
        {
            _stream << Internals::indent7 << "\"avg_time\": " << metrics.avg_time() << ",\n";
            _stream << Internals::indent7 << "\"min_time\": " << metrics.min_time() << ",\n";
            _stream << Internals::indent7 << "\"max_time\": " << metrics.max_time() << ",\n";
        }
    }
    _stream << Internals::indent7 << "\"total_time\": " << metrics.total_time() << ",\n";
    if (metrics.total_operations() > 1)
        _stream << Internals::indent7 << "\"total_operations\": " << metrics.total_operations() << ",\n";
    if (metrics.total_items() > 0)
        _stream << Internals::indent7 << "\"total_items\": " << metrics.total_items() << ",\n";
    if (metrics.total_bytes() > 0)
        _stream << Internals::indent7 << "\"total_bytes\": " << metrics.total_bytes() << ",\n";
    if (metrics.total_operations() > 1)
        _stream << Internals::indent7 << "\"operations_per_second\": " << metrics.operations_per_second() << ",\n";
    if (metrics.total_items() > 0)
        _stream << Internals::indent7 << "\"items_per_second\": " << metrics.items_per_second() << ",\n";
    if (metrics.total_bytes() > 0)
        _stream << Internals::indent7 << "\"bytes_per_second\": " << metrics.bytes_per_second() << "\n";
    _stream << Internals::indent7 << "\"custom\": [";
    if (!metrics.custom_int().empty() || !metrics.custom_uint().empty() ||
        !metrics.custom_int64().empty() || !metrics.custom_uint64().empty() ||
        !metrics.custom_flt().empty() || !metrics.custom_dbl().empty() ||
        !metrics.custom_str().empty())
    {
        std::set<std::string> names;
        for (const auto& it : metrics.custom_int())
            names.insert(it.first);
        for (const auto& it : metrics.custom_uint())
            names.insert(it.first);
        for (const auto& it : metrics.custom_int64())
            names.insert(it.first);
        for (const auto& it : metrics.custom_uint64())
            names.insert(it.first);
        for (const auto& it : metrics.custom_flt())
            names.insert(it.first);
        for (const auto& it : metrics.custom_dbl())
            names.insert(it.first);
        for (const auto& it : metrics.custom_str())
            names.insert(it.first);
        bool comma = false;
        for (const auto& name : names)
        {
            auto it_int = metrics.custom_int().find(name);
            if (it_int != metrics.custom_int().end())
            {
                if (comma)
                    _stream << ',';
                _stream << '\n' << Internals::indent8 << "{ " << '"' << it_int->first << "\": " << it_int->second << " }";
                comma = true;
            }
            auto it_uint = metrics.custom_uint().find(name);
            if (it_uint != metrics.custom_uint().end())
            {
                if (comma)
                    _stream << ',';
                _stream << '\n' << Internals::indent8 << "{ " << '"' << it_uint->first << "\": " << it_uint->second << " }";
                comma = true;
            }
            auto it_int64 = metrics.custom_int64().find(name);
            if (it_int64 != metrics.custom_int64().end())
            {
                if (comma)
                    _stream << ',';
                _stream << '\n' << Internals::indent8 << "{ " << '"' << it_int64->first << "\": " << it_int64->second << " }";
                comma = true;
            }
            auto it_uint64 = metrics.custom_uint64().find(name);
            if (it_uint64 != metrics.custom_uint64().end())
            {
                if (comma)
                    _stream << ',';
                _stream << '\n' << Internals::indent8 << "{ " << '"' << it_uint64->first << "\": " << it_uint64->second << " }";
                comma = true;
            }
            auto it_flt = metrics.custom_flt().find(name);
            if (it_flt != metrics.custom_flt().end())
            {
                if (comma)
                    _stream << ',';
                _stream << '\n' << Internals::indent8 << "{ " << '"' << it_flt->first << "\": " << it_flt->second << " }";
                comma = true;
            }
            auto it_dbl = metrics.custom_dbl().find(name);
            if (it_dbl != metrics.custom_dbl().end())
            {
                if (comma)
                    _stream << ',';
                _stream << '\n' << Internals::indent8 << "{ " << '"' << it_dbl->first << "\": " << it_dbl->second << " }";
                comma = true;
            }
            auto it_str = metrics.custom_str().find(name);
            if (it_str != metrics.custom_str().end())
            {
                if (comma)
                    _stream << ',';
                _stream << '\n' << Internals::indent8 << "{ " << '"' << it_str->first << "\": \"" << it_str->second << "\" }";
                comma = true;
            }
        }
    }
    _stream << '\n';
    _stream << Internals::indent7 << "]\n";
}

void ReporterJSON::ReportFooter()
{
    _stream << "}\n";
}

} // namespace CppBenchmark
