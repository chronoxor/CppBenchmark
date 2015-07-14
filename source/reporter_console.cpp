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
    GenerateSeparator('=');
    _stream << "CppBenchmark Library. Version " << version << std::endl;
}

void ReporterConsole::ReportSystem()
{
    GenerateSeparator('=');
    _stream << "CPU architecutre: " << System::CpuArchitecture() << std::endl;
    _stream << "CPU logical cores: " << System::CpuLogicalCores() << std::endl;
    _stream << "CPU physical cores: " << System::CpuPhysicalCores() << std::endl;
    _stream << "CPU clock speed: " << System::CpuClockSpeed() << " Mhz" << std::endl;
    _stream << "CPU Hyper-Threading: " << (System::CpuHyperThreading() ? "enabled" : "disabled") << std::endl;
    _stream << "RAM total: " << GenerateSize(System::RamTotal()) << std::endl;
    _stream << "RAM free: " << GenerateSize(System::RamFree()) << std::endl;
}

void ReporterConsole::ReportEnvironment()
{
    GenerateSeparator('=');
    _stream << "OS version: " << (Environment::Is64BitOS() ? "64-bit" : (Environment::Is32BitOS() ? "32-bit" : "<unknown>")) << std::endl;
    _stream << "Process version: " << (Environment::Is64BitProcess() ? "64-bit" : (Environment::Is32BitProcess() ? "32-bit" : "<unknown>")) << std::endl;
    _stream << "Process configuaraion: " << (Environment::IsDebug() ? "Debug" : (Environment::IsRelease() ? "Release" : "<unknown>")) << std::endl;
    time_t timestamp = Environment::Timestamp();
    _stream << "Local timestamp: " << std::asctime(std::localtime(&timestamp)) << std::endl;
    _stream << "UTC timestamp: " << std::asctime(std::gmtime(&timestamp)) << std::endl;
}

void ReporterConsole::ReportBenchmark(const Benchmark& benchmark, const Settings& settings)
{
    GenerateSeparator('=');
    _stream << "Benchmark: " << benchmark.name() << std::endl;
    _stream << "Attempts: " << settings.attempts() << std::endl;
    if (settings.iterations() > 0)
        _stream << "Iterations: " << settings.iterations() << std::endl;
    if (settings.nanoseconds() > 0)
        _stream << "Timer: " << GenerateTime(settings.nanoseconds()) << std::endl;
}

void ReporterConsole::ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics)
{
    GenerateSeparator('-');
    _stream << "Phase: " << phase.name() << std::endl;
    _stream << "Avg time: " << GenerateTime(metrics.avg_time()) << std::endl;
    _stream << "Min time: " << GenerateTime(metrics.min_time()) << std::endl;
    _stream << "Max time: " << GenerateTime(metrics.max_time()) << std::endl;
    _stream << "Total time: " << GenerateTime(metrics.total_time()) << std::endl;
    _stream << "Total iterations: " << metrics.total_iterations() << std::endl;
    _stream << "Total items: " << metrics.total_items() << std::endl;
    _stream << "Total bytes: " << GenerateSize(metrics.total_bytes()) << std::endl;
    _stream << "Iterations throughput: " << metrics.iterations_per_second() << " / second" << std::endl;
    _stream << "Items throughput: " << metrics.items_per_second() << " / second" << std::endl;
    _stream << "Bytes throughput: " << GenerateSize(metrics.bytes_per_second()) << " / second" << std::endl;
}

void ReporterConsole::ReportFooter()
{
    GenerateSeparator('=');
}

std::string ReporterConsole::GenerateSeparator(char ch)
{
    std::ostringstream stream;

    for (int i = 0; i < 79; ++i)
        stream << ch;
    stream << std::endl;

    return stream.str();
}

std::string ReporterConsole::GenerateSize(int64_t bytes)
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

std::string ReporterConsole::GenerateTime(int64_t nanoseconds)
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
