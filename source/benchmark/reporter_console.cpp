/*!
    \file reporter_console.cpp
    \brief Console reporter implementation
    \author Ivan Shynkarenka
    \date 13.07.2015
    \copyright MIT License
*/

#include "benchmark/reporter_console.h"

#include "benchmark/console.h"
#include "benchmark/environment.h"
#include "benchmark/version.h"

#include <set>
#include <sstream>

namespace CppBenchmark {

void ReporterConsole::ReportHeader()
{
    _stream << Color::DARKGREY << GenerateSeparator('=') << std::endl;
    _stream << Color::WHITE << "CppBenchmark report. Version " << version << std::endl;
}

void ReporterConsole::ReportSystem()
{
    _stream << Color::DARKGREY << GenerateSeparator('=') << std::endl;
    _stream << Color::WHITE << "CPU architecture: " << Color::LIGHTCYAN << System::CpuArchitecture() << std::endl;
    _stream << Color::WHITE << "CPU logical cores: " << Color::LIGHTGREEN << System::CpuLogicalCores() << std::endl;
    _stream << Color::WHITE << "CPU physical cores: " << Color::LIGHTGREEN << System::CpuPhysicalCores() << std::endl;
    _stream << Color::WHITE << "CPU clock speed: " << Color::LIGHTGREEN << GenerateClockSpeed(System::CpuClockSpeed()) << std::endl;
    _stream << Color::WHITE << "CPU Hyper-Threading: " << Color::LIGHTGREEN << (System::CpuHyperThreading() ? "enabled" : "disabled") << std::endl;
    _stream << Color::WHITE << "RAM total: " << Color::YELLOW << GenerateDataSize(System::RamTotal()) << std::endl;
    _stream << Color::WHITE << "RAM free: " << Color::YELLOW << GenerateDataSize(System::RamFree()) << std::endl;
}

void ReporterConsole::ReportEnvironment()
{
    _stream << Color::DARKGREY << GenerateSeparator('=') << std::endl;
    _stream << Color::WHITE << "OS version: " << Color::DARKGREY << Environment::OSVersion() << std::endl;
    _stream << Color::WHITE << "OS bits: " << Color::DARKGREY << (Environment::Is64BitOS() ? "64-bit" : (Environment::Is32BitOS() ? "32-bit" : "<unknown>")) << std::endl;
    _stream << Color::WHITE << "Process bits: " << Color::DARKGREY << (Environment::Is64BitProcess() ? "64-bit" : (Environment::Is32BitProcess() ? "32-bit" : "<unknown>")) << std::endl;
    _stream << Color::WHITE << "Process configuration: " << Color::DARKGREY << (Environment::IsDebug() ? "debug" : (Environment::IsRelease() ? "release" : "<unknown>")) << std::endl;
    time_t timestamp = Environment::Timestamp();
    _stream << Color::WHITE << "Local timestamp: " << Color::DARKGREY << std::asctime(std::localtime(&timestamp));
    _stream << Color::WHITE << "UTC timestamp: " << Color::DARKGREY << std::asctime(std::gmtime(&timestamp));
}

void ReporterConsole::ReportBenchmark(const BenchmarkBase& benchmark, const Settings& settings)
{
    _stream << Color::DARKGREY << GenerateSeparator('=') << std::endl;
    _stream << Color::WHITE << "Benchmark: " << Color::LIGHTCYAN << benchmark.name() << std::endl;
    _stream << Color::WHITE << "Attempts: " << Color::DARKGREY << settings.attempts() << std::endl;
    if (settings.duration() > 0)
        _stream << Color::WHITE << "Duration: " << Color::DARKGREY << settings.duration() << " seconds" << std::endl;
    if (settings.operations() > 0)
        _stream << Color::WHITE << "Operations: " << Color::DARKGREY << settings.operations() << std::endl;
}

void ReporterConsole::ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics)
{
    _stream << Color::DARKGREY << GenerateSeparator('-') << std::endl;
    _stream << Color::WHITE << "Phase: " << Color::LIGHTCYAN << phase.name() << std::endl;
    if (metrics.total_operations() > 1)
    {
        if (metrics.latency())
        {
            _stream << Color::WHITE << "Latency (Min): " << Color::YELLOW << GenerateTimePeriod(metrics.min_latency()) << "/op" << std::endl;
            _stream << Color::WHITE << "Latency (Max): " << Color::YELLOW << GenerateTimePeriod(metrics.max_latency()) << "/op" << std::endl;
            _stream << Color::WHITE << "Latency (Mean): " << Color::YELLOW << metrics.mean_latency() << std::endl;
            _stream << Color::WHITE << "Latency (StDv): " << Color::YELLOW << metrics.stdv_latency() << std::endl;
        }
        else
        {
            _stream << Color::WHITE << "Average time: " << Color::YELLOW << GenerateTimePeriod(metrics.avg_time()) << "/op" << std::endl;
            _stream << Color::WHITE << "Minimal time: " << Color::YELLOW << GenerateTimePeriod(metrics.min_time()) << "/op" << std::endl;
            _stream << Color::WHITE << "Maximal time: " << Color::YELLOW << GenerateTimePeriod(metrics.max_time()) << "/op" << std::endl;
        }
    }
    _stream << Color::WHITE << "Total time: " << Color::LIGHTRED << GenerateTimePeriod(metrics.total_time()) << std::endl;
    if (metrics.total_operations() > 1)
        _stream << Color::WHITE << "Total operations: " << Color::LIGHTGREEN << metrics.total_operations() << std::endl;
    if (metrics.total_items() > 0)
        _stream << Color::WHITE << "Total items: " << Color::LIGHTMAGENTA << metrics.total_items() << std::endl;
    if (metrics.total_bytes() > 0)
        _stream << Color::WHITE << "Total bytes: " << Color::MAGENTA << GenerateDataSize(metrics.total_bytes()) << std::endl;
    if (metrics.total_operations() > 1)
        _stream << Color::WHITE << "Operations throughput: " << Color::LIGHTGREEN << metrics.operations_per_second() << " ops/s" << std::endl;
    if (metrics.total_items() > 0)
        _stream << Color::WHITE << "Items throughput: " << Color::LIGHTMAGENTA << metrics.items_per_second() << " items/s" << std::endl;
    if (metrics.total_bytes() > 0)
        _stream << Color::WHITE << "Bytes throughput: " << Color::MAGENTA << GenerateDataSize(metrics.bytes_per_second()) << "/s" << std::endl;
    if ((metrics.custom_int().size() > 0) || (metrics.custom_uint().size() > 0) ||
        (metrics.custom_int64().size() > 0) || (metrics.custom_uint64().size() > 0) ||
        (metrics.custom_flt().size() > 0) || (metrics.custom_dbl().size() > 0) ||
        (metrics.custom_str().size() > 0))
    {
        _stream << Color::WHITE << "Custom values: " << std::endl;
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
        for (const auto& name : names)
        {
            auto it_int = metrics.custom_int().find(name);
            if (it_int != metrics.custom_int().end())
                _stream << Color::DARKGREY << '\t' << it_int->first << ": " << Color::GREY << it_int->second << std::endl;
            auto it_uint = metrics.custom_uint().find(name);
            if (it_uint != metrics.custom_uint().end())
                _stream << Color::DARKGREY << '\t' << it_uint->first << ": " << Color::GREY << it_uint->second << std::endl;
            auto it_int64 = metrics.custom_int64().find(name);
            if (it_int64 != metrics.custom_int64().end())
                _stream << Color::DARKGREY << '\t' << it_int64->first << ": " << Color::GREY << it_int64->second << std::endl;
            auto it_uint64 = metrics.custom_uint64().find(name);
            if (it_uint64 != metrics.custom_uint64().end())
                _stream << Color::DARKGREY << '\t' << it_uint64->first << ": " << Color::GREY << it_uint64->second << std::endl;
            auto it_flt = metrics.custom_flt().find(name);
            if (it_flt != metrics.custom_flt().end())
                _stream << Color::DARKGREY << '\t' << it_flt->first << ": " << Color::GREY << it_flt->second << std::endl;
            auto it_dbl = metrics.custom_dbl().find(name);
            if (it_dbl != metrics.custom_dbl().end())
                _stream << Color::DARKGREY << '\t' << it_dbl->first << ": " << Color::GREY << it_dbl->second << std::endl;
            auto it_str = metrics.custom_str().find(name);
            if (it_str != metrics.custom_str().end())
                _stream << Color::DARKGREY << '\t' << it_str->first << ": " << Color::GREY << it_str->second << std::endl;
        }
    }
}

void ReporterConsole::ReportFooter()
{
    _stream << Color::DARKGREY << GenerateSeparator('=') << std::endl;
}

std::string ReporterConsole::GenerateSeparator(char ch)
{
    return std::string(79, ch);
}

std::string ReporterConsole::GenerateClockSpeed(int64_t hertz)
{
    std::ostringstream stream;

    int64_t abs_hertz = std::abs(hertz);

    if (abs_hertz >= 1000000000)
    {
        int64_t gigahertz = hertz / 1000000000;
        int64_t megahertz = (hertz % 1000000000) / 1000000;
        stream << gigahertz << '.' << ((megahertz < 100) ? "0" : "") << ((megahertz < 10) ? "0" : "") << megahertz << " GHz";
    }
    else if (abs_hertz >= 1000000)
    {
        int64_t megahertz = hertz / 1000000;
        int64_t kilohertz = (hertz % 1000000) / 1000;
        stream << megahertz << '.' << ((kilohertz < 100) ? "0" : "") << ((kilohertz < 10) ? "0" : "") << kilohertz << " MHz";
    }
    else if (abs_hertz >= 1000)
    {
        int64_t kilohertz = hertz / 1000;
        hertz = hertz % 1000;
        stream << kilohertz << '.' << ((hertz < 100) ? "0" : "") << ((hertz < 10) ? "0" : "") << hertz << " kHz";
    }
    else
        stream << hertz << " Hz";

    return stream.str();
}

std::string ReporterConsole::GenerateDataSize(int64_t bytes)
{
    std::ostringstream stream;

    int64_t abs_bytes = std::abs(bytes);

    if (abs_bytes >= (1024ll * 1024ll * 1024ll * 1024ll))
    {
        int64_t tb = bytes / (1024ll * 1024ll * 1024ll * 1024ll);
        int64_t gb = (bytes % (1024ll * 1024ll * 1024ll * 1024ll)) / (1024 * 1024 * 1024);
        stream << tb << '.' << ((gb < 100) ? "0" : "") << ((gb < 10) ? "0" : "") << gb << " TiB";
    }
    else if (abs_bytes >= (1024 * 1024 * 1024))
    {
        int64_t gb = bytes / (1024 * 1024 * 1024);
        int64_t mb = (bytes % (1024 * 1024 * 1024)) / (1024 * 1024);
        stream << gb << '.' << ((mb < 100) ? "0" : "") << ((mb < 10) ? "0" : "") << mb << " GiB";
    }
    else if (abs_bytes >= (1024 * 1024))
    {
        int64_t mb = bytes / (1024 * 1024);
        int64_t kb = (bytes % (1024 * 1024)) / 1024;
        stream << mb << '.' << ((kb < 100) ? "0" : "") << ((kb < 10) ? "0" : "") << kb << " MiB";
    }
    else if (abs_bytes >= 1024)
    {
        int64_t kb = bytes / 1024;
        bytes = bytes % 1024;
        stream << kb << '.' << ((bytes < 100) ? "0" : "") << ((bytes < 10) ? "0" : "") << bytes << " KiB";
    }
    else
        stream << bytes << " bytes";

    return stream.str();
}

std::string ReporterConsole::GenerateTimePeriod(int64_t nanoseconds)
{
    std::ostringstream stream;

    int64_t abs_nanoseconds = std::abs(nanoseconds);

    if (abs_nanoseconds >= (60 * 60 * 1000000000ll))
    {
        int64_t hours = nanoseconds / (60 * 60 * 1000000000ll);
        int64_t minutes = ((nanoseconds % (60 * 60 * 1000000000ll)) / 1000000000) / 60;
        int64_t seconds = ((nanoseconds % (60 * 60 * 1000000000ll)) / 1000000000) % 60;
        int64_t milliseconds = ((nanoseconds % (60 * 60 * 1000000000ll)) % 1000000000) / 1000000;
        stream << hours << ':' << ((minutes < 10) ? "0" : "") << minutes << ':' << ((seconds < 10) ? "0" : "") << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds << " h";
    }
    else if (abs_nanoseconds >= (60 * 1000000000ll))
    {
        int64_t minutes = nanoseconds / (60 * 1000000000ll);
        int64_t seconds = (nanoseconds % (60 * 1000000000ll)) / 1000000000;
        int64_t milliseconds = ((nanoseconds % (60 * 1000000000ll)) % 1000000000) / 1000000;
        stream << minutes << ':' << ((seconds < 10) ? "0" : "") << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds << " m";
    }
    else if (abs_nanoseconds >= 1000000000)
    {
        int64_t seconds = nanoseconds / 1000000000;
        int64_t milliseconds = (nanoseconds % 1000000000) / 1000000;
        stream << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds << " s";
    }
    else if (abs_nanoseconds >= 1000000)
    {
        int64_t milliseconds = nanoseconds / 1000000;
        int64_t microseconds = (nanoseconds % 1000000) / 1000;
        stream << milliseconds << '.' << ((microseconds < 100) ? "0" : "") << ((microseconds < 10) ? "0" : "") << microseconds << " ms";
    }
    else if (abs_nanoseconds >= 1000)
    {
        int64_t microseconds = nanoseconds / 1000;
        nanoseconds = nanoseconds % 1000;
        stream << microseconds << '.' << ((nanoseconds < 100) ? "0" : "") << ((nanoseconds < 10) ? "0" : "") << nanoseconds << " mcs";
    }
    else
        stream << nanoseconds << " ns";

    return stream.str();
}

} // namespace CppBenchmark
