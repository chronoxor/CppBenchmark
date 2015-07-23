//
// Created by Ivan Shynkarenka on 13.07.2015.
//

#include "reporter_console.h"

#include <ctime>
#include <iomanip>
#include <sstream>

#include "environment.h"
#include "system.h"
#include "version.h"

namespace CppBenchmark {

void ReporterConsole::ReportHeader()
{
    _stream << GenerateSeparator('=') << std::endl;
    _stream << "CppBenchmark Library. Version " << version << std::endl;
}

void ReporterConsole::ReportSystem()
{
    _stream << GenerateSeparator('=') << std::endl;
    _stream << "CPU architecutre: " << System::CpuArchitecture() << std::endl;
    _stream << "CPU logical cores: " << System::CpuLogicalCores() << std::endl;
    _stream << "CPU physical cores: " << System::CpuPhysicalCores() << std::endl;
    _stream << "CPU clock speed: " << GenerateClockSpeed(System::CpuClockSpeed()) << std::endl;
    _stream << "CPU Hyper-Threading: " << (System::CpuHyperThreading() ? "enabled" : "disabled") << std::endl;
    _stream << "RAM total: " << GenerateSize(System::RamTotal()) << std::endl;
    _stream << "RAM free: " << GenerateSize(System::RamFree()) << std::endl;
}

void ReporterConsole::ReportEnvironment()
{
    _stream << GenerateSeparator('=') << std::endl;
    _stream << "OS version: " << (Environment::Is64BitOS() ? "64-bit" : (Environment::Is32BitOS() ? "32-bit" : "<unknown>")) << std::endl;
    _stream << "Process version: " << (Environment::Is64BitProcess() ? "64-bit" : (Environment::Is32BitProcess() ? "32-bit" : "<unknown>")) << std::endl;
    _stream << "Process configuaraion: " << (Environment::IsDebug() ? "Debug" : (Environment::IsRelease() ? "Release" : "<unknown>")) << std::endl;
    time_t timestamp = Environment::Timestamp();
    _stream << "Local timestamp: " << std::asctime(std::localtime(&timestamp));
    _stream << "UTC timestamp: " << std::asctime(std::gmtime(&timestamp));
}

void ReporterConsole::ReportBenchmark(const Benchmark& benchmark, const Settings& settings)
{
    _stream << GenerateSeparator('=') << std::endl;
    _stream << "Benchmark: " << benchmark.name() << std::endl;
    _stream << "Metrics: " << to_string(_type) << std::endl;
    _stream << "Attempts: " << settings.attempts() << std::endl;
    if (settings.iterations() > 0)
        _stream << "Iterations: " << settings.iterations() << std::endl;
    if (settings.nanoseconds() > 0)
        _stream << "Timer: " << GenerateTime(settings.nanoseconds()) << std::endl;
}

void ReporterConsole::ReportPhase(const PhaseCore& phase, const PhaseMetrics& best, const PhaseMetrics& worst)
{
    _stream << GenerateSeparator('-') << std::endl;
    _stream << "Phase: " << phase.name() << std::endl;
    _stream << "Avg time: " << GenerateTime(ReportValue(best.avg_time(), worst.avg_time())) << std::endl;
    _stream << "Min time: " << GenerateTime(ReportValue(best.min_time(), worst.min_time())) << std::endl;
    _stream << "Max time: " << GenerateTime(ReportValue(best.max_time(), worst.max_time())) << std::endl;
    _stream << "Total time: " << GenerateTime(ReportValue(best.total_time(), worst.total_time())) << std::endl;
    if ((best.total_iterations() > 1) || (worst.total_iterations() > 1))
        _stream << "Total iterations: " << ReportValue(best.total_iterations(), worst.total_iterations()) << std::endl;
    if ((best.total_items() > 0) || (worst.total_items() > 0))
        _stream << "Total items: " << ReportValue(best.total_items(), worst.total_items()) << std::endl;
    if ((best.total_bytes() > 0) || (worst.total_bytes() > 0))
        _stream << "Total bytes: " << GenerateSize(ReportValue(best.total_bytes(), worst.total_bytes())) << std::endl;
    if ((best.total_iterations() > 1) || (worst.total_iterations() > 1))
        _stream << "Iterations throughput: " << std::fixed << std::setprecision(3) << ReportValue(best.iterations_per_second(), worst.iterations_per_second()) << " / second" << std::endl;
    if ((best.total_items() > 0) || (worst.total_items() > 0))
        _stream << "Items throughput: " << std::fixed << std::setprecision(3) << ReportValue(best.items_per_second(), worst.items_per_second()) << " / second" << std::endl;
    if ((best.total_bytes() > 0) || (worst.total_bytes() > 0))
        _stream << "Bytes throughput: " << GenerateSize((int64_t)ReportValue(best.bytes_per_second(), worst.bytes_per_second())) << " / second" << std::endl;
}

void ReporterConsole::ReportFooter()
{
    _stream << GenerateSeparator('=') << std::endl;
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
