/*!
    \file launcher_console.cpp
    \brief Console launcher implementation
    \author Ivan Shynkarenka
    \date 16.07.2015
    \copyright MIT License
*/

#include "benchmark/launcher_console.h"

#include "benchmark/console.h"
#include "benchmark/reporter_console.h"
#include "benchmark/reporter_csv.h"
#include "benchmark/reporter_json.h"
#include "benchmark/version.h"

#include <iomanip>
#include <regex>

#include <OptionParser.h>

namespace CppBenchmark {

void LauncherConsole::Initialize(const int argc, char const* const* const argv)
{
    auto parser = optparse::OptionParser().version(version);

    const char* output[] = { "console", "csv", "json" };

    parser.add_option("-f", "--filter").dest("filter").help("Filter benchmarks by the given regexp pattern");
    parser.add_option("-l", "--list").dest("list").action("store_true").help("List all avaliable benchmarks");
    parser.add_option("-o", "--output").dest("output").choices(&output[0], &output[3]).set_default(output[0]).help("Output format (console, csv, json). Default: %default");
    parser.add_option("-q", "--quiet").dest("quiet").action("store_true").help("Launch in quiet mode. No progress will be shown!");
    parser.add_option("-r", "--histograms").dest("histograms").action("store").type("int").set_default(0).help("Create High Dynamic Range (HDR) Histogram files with a given resolution. Default: %default");

    optparse::Values options = parser.parse_args(argc, argv);

    // Check for double initialization
    if (_init)
    {
        parser.error("Console launcher double initialization!");
    }

    // Print help
    if (options.get("help"))
    {
        parser.print_help();
        exit(EXIT_SUCCESS);
    }

    // Setup console launcher parameters
    _list = options.get("list");
    _quiet = options.get("quiet");
    _histograms = (int32_t)options.get("histograms");
    if (options.is_set("filter"))
        _filter = options["filter"];
    if (options.is_set("output"))
        _output = options["output"];

    // Update initialization flag
    _init = true;
}

void LauncherConsole::Execute()
{
    if (_list)
    {
        // List all suitable benchmarks
        std::regex matcher(_filter);
        for (const auto& benchmark : _benchmarks)
        {
            // Match benchmark name with the given pattern
            if (_filter.empty() || std::regex_match(benchmark->name(), matcher))
                std::cout << benchmark->name() << std::endl;
        }
    }
    else
    {
        // Launch all suitable benchmarks
        Launcher::Launch(_filter);
    }
}

void LauncherConsole::Report() const
{
    if (_output == "console")
    {
        ReporterConsole reporter(std::cout);
        Launcher::Report(reporter);
    }
    else if (_output == "csv")
    {
        ReporterCSV reporter(std::cout);
        Launcher::Report(reporter);
    }
    else if (_output == "json")
    {
        ReporterJSON reporter(std::cout);
        Launcher::Report(reporter);
    }

    // Report histograms
    if (_histograms > 0)
        Launcher::ReportHistograms(_histograms);
}

void LauncherConsole::onLaunching(int current, int total, const BenchmarkBase& benchmark, const Context& context, int attempt)
{
    if (_quiet)
        return;

    std::cerr << Color::DARKGREY << "[" << std::setw(3) << (100 * current / total) << "%] " << Color::GREY << "Launching " << Color::LIGHTCYAN << benchmark.name() << context.description() << Color::GREY << ". Attempt " << Color::WHITE << attempt << Color::GREY << "...";
}

void LauncherConsole::onLaunched(int current, int total, const BenchmarkBase& benchmark, const Context& context, int attempt)
{
    if (_quiet)
        return;

    std::cerr << Color::LIGHTGREEN << "Done!" << std::endl;
}

} // namespace CppBenchmark
