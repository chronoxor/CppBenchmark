//
// Created by Ivan Shynkarenka on 13.07.2015.
//

#include "reporter_console.h"

#include <ctime>
#include <sstream>

#include "environment.h"
#include "system.h"
#include "version.h"

namespace CppBenchmark {

void ReporterConsole::ReportHeader()
{
    ReportSeparator('=');
    _stream << "CppBenchmark Library. Version " << version << std::endl;
}

void ReporterConsole::ReportSystem()
{
    ReportSeparator('=');
    _stream << "CPU architecutre: " << System::CpuArchitecture() << std::endl;
    _stream << "CPU logical cores: " << System::CpuLogicalCores() << std::endl;
    _stream << "CPU physical cores: " << System::CpuPhysicalCores() << std::endl;
    _stream << "CPU clock speed: " << System::CpuClockSpeed() << " Mhz" << std::endl;
    _stream << "CPU hyperthreading: " << (System::CpuHyperThreading() ? "enabled" : "disabled") << std::endl;
    _stream << "RAM total: " << System::RamTotal() << " bytes" << std::endl;
    _stream << "RAM free: " << System::RamFree() << " bytes" << std::endl;
}

void ReporterConsole::ReportEnvironment()
{
    ReportSeparator('=');
    _stream << "OS version: " << (Environment::Is64BitOS() ? "64-bit" : (Environment::Is32BitOS() ? "32-bit" : "<unknown>")) << std::endl;
    _stream << "Process version: " << (Environment::Is64BitProcess() ? "64-bit" : (Environment::Is32BitProcess() ? "32-bit" : "<unknown>")) << std::endl;
    _stream << "Process configuaraion: " << (Environment::IsDebug() ? "Debug" : (Environment::IsRelease() ? "Release" : "<unknown>")) << std::endl;
    time_t timestamp = Environment::Timestamp();
    _stream << "Local timestamp: " << std::asctime(std::localtime(&timestamp)) << std::endl;
    _stream << "UTC timestamp: " << std::asctime(std::gmtime(&timestamp)) << std::endl;
}

void ReporterConsole::ReportBenchmark(const Benchmark& benchmark, const Settings& settings)
{
    ReportSeparator('=');
    _stream << "Benchmark: " << benchmark.name() << std::endl;
    _stream << "Attempts: " << settings.attempts() << std::endl;
    if (settings.iterations() > 0)
        _stream << "Iterations: " << settings.iterations() << std::endl;
    if (settings.nanoseconds() > 0)
        _stream << "Timer: " << ReportTime(settings.nanoseconds()) << std::endl;
}

void ReporterConsole::ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics)
{
    ReportSeparator('-');
    _stream << "Phase: " << phase.name() << std::endl;
    _stream << "Avg time: " << ReportTime(metrics.avg_time()) << std::endl;
    _stream << "Min time: " << ReportTime(metrics.min_time()) << std::endl;
    _stream << "Max time: " << ReportTime(metrics.max_time()) << std::endl;
    _stream << "Total time: " << ReportTime(metrics.total_time()) << std::endl;
    _stream << "Total iterations: " << metrics.total_iterations() << std::endl;
    _stream << "Total items: " << metrics.total_items() << std::endl;
    _stream << "Total bytes: " << ReportSize(metrics.total_bytes()) << std::endl;
    _stream << "Iterations throughput: " << metrics.iterations_per_second() << " / second" << std::endl;
    _stream << "Items throughput: " << metrics.items_per_second() << " / second" << std::endl;
    _stream << "Bytes throughput: " << ReportSize(metrics.bytes_per_second()) << " / second" << std::endl;
}

void ReporterConsole::ReportFooter()
{
    ReportSeparator('=');
}

std::string ReporterConsole::ReportSeparator(char ch)
{
    std::ostringstream stream;

    for (int i = 0; i < 79; ++i)
        stream << ch;
    stream << std::endl;

    return stream.str();
}

std::string ReporterConsole::ReportSize(int64_t bytes)
{
    std::ostringstream stream;

    if (bytes > 1099511627776ll) {
        int64_t tb = bytes / 1099511627776ll;
        int64_t gb = (bytes % 1099511627776ll) / 1024;
        stream << tb << '.' << ((gb < 100) ? "0" : "") << ((gb < 10) ? "0" : "") << gb << " TiB";
    }
    else if (bytes > 1073741824ll) {
        int64_t gb = bytes / 1073741824ll;
        int64_t mb = (bytes % 1073741824ll) / 1024;
        stream << gb << '.' << ((mb < 100) ? "0" : "") << ((mb < 10) ? "0" : "") << mb << " GiB";
    }
    else if (bytes > 1048576ll) {
        int64_t mb = bytes / 1048576ll;
        int64_t kb = (bytes % 1048576ll) / 1024;
        stream << mb << '.' << ((kb < 100) ? "0" : "") << ((kb < 10) ? "0" : "") << kb << " MiB";
    }
    else if (bytes > 1024ll) {
        int64_t kb = bytes / 1024;
        bytes = bytes % 1024;
        stream << kb << '.' << ((bytes < 100) ? "0" : "") << ((bytes < 10) ? "0" : "") << bytes << " KiB";
    }
    else
        stream << bytes << " bytes";

    return stream.str();
}

std::string ReporterConsole::ReportTime(int64_t nanoseconds)
{
    std::ostringstream stream;

    if (nanoseconds > 216000000000000ll) {
        int64_t hours = nanoseconds / (3600000000000ll);
        int64_t minutes = (nanoseconds % (3600000000000ll)) / 60;
        int64_t seconds = ((nanoseconds % (3600000000000ll)) % 60) / 1000000000ll;
        int64_t milliseconds = (((nanoseconds % (3600000000000ll)) % 60) % 1000000000ll) / 1000000ll;
        stream << hours << ':' << ((minutes < 10) ? "0" : "") << minutes << ':' << ((seconds < 10) ? "0" : "") << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds;
    }
    else if (nanoseconds > 3600000000000ll) {
        int64_t minutes = nanoseconds / (60000000000ll);
        int64_t seconds = (nanoseconds % (60000000000ll)) / 1000000000ll;
        int64_t milliseconds = ((nanoseconds % (60000000000ll)) % 1000000000ll) / 1000000ll;
        stream << minutes << ((seconds < 10) ? "0" : "") << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds;
    }
    else if (nanoseconds > 60000000000ll) {
        int64_t seconds = nanoseconds / 1000000000ll;
        int64_t milliseconds = (nanoseconds % 1000000000ll) / 1000000ll;
        stream << seconds << '.' << ((milliseconds < 100) ? "0" : "") << ((milliseconds < 10) ? "0" : "") << milliseconds << " s";
    }
    else if (nanoseconds > 1000000ll) {
        int64_t milliseconds = nanoseconds / 1000000ll;
        int64_t microseconds = (nanoseconds % 1000000ll) / 1000;
        stream << milliseconds << '.' << ((microseconds < 100) ? "0" : "") << ((microseconds < 10) ? "0" : "") << microseconds << " ms";
    }
    else if (nanoseconds > 1000ll) {
        int64_t microseconds = nanoseconds / 1000;
        nanoseconds = nanoseconds % 1000;
        stream << microseconds << '.' << ((nanoseconds < 100) ? "0" : "") << ((nanoseconds < 10) ? "0" : "") << nanoseconds << " mcs";
    }
    else
        _stream << nanoseconds << " ns";

    return stream.str();
}

} // namespace CppBenchmark
