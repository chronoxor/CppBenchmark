/*!
    \file reporter_console.cpp
    \brief Console reporter implementation
    \author Ivan Shynkarenka
    \date 13.07.2015
    \copyright MIT License
*/

#include "reporter_console.h"

#include <set>
#include <sstream>

#include "console.h"
#include "environment.h"
#include "version.h"

namespace CppBenchmark {

void ReporterConsole::ReportHeader()
{
    _stream << DARKGREY << GenerateSeparator('=') << std::endl;
    _stream << WHITE << "CppBenchmark report. Version " << version << std::endl;
}

void ReporterConsole::ReportSystem()
{
    _stream << DARKGREY << GenerateSeparator('=') << std::endl;
    _stream << WHITE << "CPU architecutre: " << LIGHTCYAN << System::CpuArchitecture() << std::endl;
    _stream << WHITE << "CPU logical cores: " << LIGHTGREEN << System::CpuLogicalCores() << std::endl;
    _stream << WHITE << "CPU physical cores: " << LIGHTGREEN << System::CpuPhysicalCores() << std::endl;
    _stream << WHITE << "CPU clock speed: " << LIGHTGREEN << GenerateClockSpeed(System::CpuClockSpeed()) << std::endl;
    _stream << WHITE << "CPU Hyper-Threading: " << LIGHTGREEN << (System::CpuHyperThreading() ? "enabled" : "disabled") << std::endl;
    _stream << WHITE << "RAM total: " << YELLOW << GenerateDataSize(System::RamTotal()) << std::endl;
    _stream << WHITE << "RAM free: " << YELLOW << GenerateDataSize(System::RamFree()) << std::endl;
}

void ReporterConsole::ReportEnvironment()
{
    _stream << DARKGREY << GenerateSeparator('=') << std::endl;
    _stream << WHITE << "OS version: " << DARKGREY << Environment::OSVersion() << std::endl;
    _stream << WHITE << "OS bits: " << DARKGREY << (Environment::Is64BitOS() ? "64-bit" : (Environment::Is32BitOS() ? "32-bit" : "<unknown>")) << std::endl;
    _stream << WHITE << "Process bits: " << DARKGREY << (Environment::Is64BitProcess() ? "64-bit" : (Environment::Is32BitProcess() ? "32-bit" : "<unknown>")) << std::endl;
    _stream << WHITE << "Process configuaraion: " << DARKGREY << (Environment::IsDebug() ? "debug" : (Environment::IsRelease() ? "release" : "<unknown>")) << std::endl;
    time_t timestamp = Environment::Timestamp();
    _stream << WHITE << "Local timestamp: " << DARKGREY << std::asctime(std::localtime(&timestamp));
    _stream << WHITE << "UTC timestamp: " << DARKGREY << std::asctime(std::gmtime(&timestamp));
}

void ReporterConsole::ReportBenchmark(const Benchmark& benchmark, const Settings& settings)
{
    _stream << DARKGREY << GenerateSeparator('=') << std::endl;
    _stream << WHITE << "Benchmark: " << LIGHTCYAN << benchmark.name() << std::endl;
    _stream << WHITE << "Attempts: " << DARKGREY << settings.attempts() << std::endl;
    if (!settings.infinite())
        _stream << WHITE << "Iterations: " << DARKGREY << settings.iterations() << std::endl;
}

void ReporterConsole::ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics)
{
    _stream << DARKGREY << GenerateSeparator('-') << std::endl;
    _stream << WHITE << "Phase: " << LIGHTCYAN << phase.name() << std::endl;
    if (metrics.total_iterations() > 1) {
        _stream << WHITE << "Average time: " << YELLOW << GenerateTimePeriod(metrics.avg_time()) << " / iteration" << std::endl;
        _stream << WHITE << "Minimal time: " << YELLOW << GenerateTimePeriod(metrics.min_time()) << " / iteration" << std::endl;
        _stream << WHITE << "Maximal time: " << YELLOW << GenerateTimePeriod(metrics.max_time()) << " / iteration" << std::endl;
    }
    _stream << WHITE << "Total time: " << LIGHTRED << GenerateTimePeriod(metrics.total_time()) << std::endl;
    if (metrics.total_iterations() > 1)
        _stream << WHITE << "Total iterations: " << LIGHTGREEN << metrics.total_iterations() << std::endl;
    if (metrics.total_items() > 0)
        _stream << WHITE << "Total items: " << LIGHTMAGENTA << metrics.total_items() << std::endl;
    if (metrics.total_bytes() > 0)
        _stream << WHITE << "Total bytes: " << MAGENTA << GenerateDataSize(metrics.total_bytes()) << std::endl;
    if (metrics.total_iterations() > 1)
        _stream << WHITE << "Iterations throughput: " << LIGHTGREEN << metrics.iterations_per_second() << " / second" << std::endl;
    if (metrics.total_items() > 0)
        _stream << WHITE << "Items throughput: " << LIGHTMAGENTA << metrics.items_per_second() << " / second" << std::endl;
    if (metrics.total_bytes() > 0)
        _stream << WHITE << "Bytes throughput: " << MAGENTA << GenerateDataSize(metrics.bytes_per_second()) << " / second" << std::endl;
    if ((metrics.custom_int().size() > 0) || (metrics.custom_str().size() > 0)) {
        _stream << WHITE << "Custom values: " << std::endl;
        std::set<std::string> names;
        for (auto it : metrics.custom_int())
            names.insert(it.first);
        for (auto it : metrics.custom_str())
            names.insert(it.first);
        for (auto name : names) {
            auto it_int = metrics.custom_int().find(name);
            if (it_int != metrics.custom_int().end())
                _stream << DARKGREY << '\t' << it_int->first << ": " << GREY << it_int->second << std::endl;
            auto it_str = metrics.custom_str().find(name);
            if (it_str != metrics.custom_str().end())
                _stream << DARKGREY << '\t' << it_str->first << ": " << GREY << it_str->second << std::endl;
        }
    }
}

void ReporterConsole::ReportFooter()
{
    _stream << DARKGREY << GenerateSeparator('=') << std::endl;
}

std::string ReporterConsole::GenerateSeparator(char ch)
{
    return std::string(79, ch);
}

std::string ReporterConsole::GenerateClockSpeed(int64_t hertz)
{
    std::ostringstream stream;

    if (hertz >= 1000000000ll) {
        int64_t gigahertz = hertz / 1000000000ll;
        int64_t megahertz = (hertz % 1000000000ll) / 1000000ll;
        stream << gigahertz << '.' << ((megahertz < 100) ? "0" : "") << ((megahertz < 10) ? "0" : "") << megahertz << " GHz";
    }
    else if (hertz >= 1000000) {
        int64_t megahertz = hertz / 1000000;
        int64_t kilohertz = (hertz % 1000000) / 1000;
        stream << megahertz << '.' << ((kilohertz < 100) ? "0" : "") << ((kilohertz < 10) ? "0" : "") << kilohertz << " MHz";
    }
    else if (hertz >= 1000) {
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

    if (bytes >= (1024ll * 1024ll * 1024ll * 1024ll)) {
        int64_t tb = bytes / (1024ll * 1024ll * 1024ll * 1024ll);
        int64_t gb = (bytes % (1024ll * 1024ll * 1024ll * 1024ll)) / (1024 * 1024 * 1024);
        stream << tb << '.' << ((gb < 100) ? "0" : "") << ((gb < 10) ? "0" : "") << gb << " TiB";
    }
    else if (bytes >= (1024 * 1024 * 1024)) {
        int64_t gb = bytes / (1024 * 1024 * 1024);
        int64_t mb = (bytes % (1024 * 1024 * 1024)) / (1024 * 1024);
        stream << gb << '.' << ((mb < 100) ? "0" : "") << ((mb < 10) ? "0" : "") << mb << " GiB";
    }
    else if (bytes >= (1024 * 1024)) {
        int64_t mb = bytes / (1024 * 1024);
        int64_t kb = (bytes % (1024 * 1024)) / 1024;
        stream << mb << '.' << ((kb < 100) ? "0" : "") << ((kb < 10) ? "0" : "") << kb << " MiB";
    }
    else if (bytes >= 1024) {
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

    if (nanoseconds >= (60 * 60 * 1000000000ll)) {
        int64_t hours = nanoseconds / (60 * 60 * 1000000000ll);
        int64_t minutes = ((nanoseconds % (60 * 60 * 1000000000ll)) / 1000000000ll) / 60;
        int64_t seconds = ((nanoseconds % (60 * 60 * 1000000000ll)) / 1000000000ll) % 60;
        int64_t milliseconds = ((nanoseconds % (60 * 60 * 1000000000ll)) % 1000000000ll) / 1000000ll;
        stream << hours << ':' << ((minutes < 10) ? "0" : "") << minutes << ':' << ((seconds < 10) ? "0" : "") << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds << " h";
    }
    else if (nanoseconds >= (60 * 1000000000ll)) {
        int64_t minutes = nanoseconds / (60 * 1000000000ll);
        int64_t seconds = (nanoseconds % (60 * 1000000000ll)) / 1000000000ll;
        int64_t milliseconds = ((nanoseconds % (60 * 1000000000ll)) % 1000000000ll) / 1000000ll;
        stream << minutes << ':' << ((seconds < 10) ? "0" : "") << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds << " m";
    }
    else if (nanoseconds >= 1000000000ll) {
        int64_t seconds = nanoseconds / 1000000000ll;
        int64_t milliseconds = (nanoseconds % 1000000000ll) / 1000000ll;
        stream << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds << " s";
    }
    else if (nanoseconds >= 1000000) {
        int64_t milliseconds = nanoseconds / 1000000;
        int64_t microseconds = (nanoseconds % 1000000) / 1000;
        stream << milliseconds << '.' << ((microseconds < 100) ? "0" : "") << ((microseconds < 10) ? "0" : "") << microseconds << " ms";
    }
    else if (nanoseconds >= 1000) {
        int64_t microseconds = nanoseconds / 1000;
        nanoseconds = nanoseconds % 1000;
        stream << microseconds << '.' << ((nanoseconds < 100) ? "0" : "") << ((nanoseconds < 10) ? "0" : "") << nanoseconds << " mcs";
    }
    else
        stream << nanoseconds << " ns";

    return stream.str();
}

} // namespace CppBenchmark
