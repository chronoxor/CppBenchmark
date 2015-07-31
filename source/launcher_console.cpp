/*!
    \file launcher_console.cpp
    \brief Console launcher implementation
    \author Ivan Shynkarenka
    \date 16.07.2015
    \copyright MIT License
*/

#include "launcher_console.h"

#include <iomanip>
#include <regex>

#include "console.h"
#include "option_parser.h"
#include "reporter_console.h"
#include "reporter_csv.h"
#include "reporter_json.h"
#include "version.h"

namespace CppBenchmark {

void LauncherConsole::Initialize(const int argc, char const* const* const argv)
{
    auto parser = optparse::OptionParser().version(version);

    const char* output[] = { "console", "csv", "json" };

    parser.add_option("-h", "--help").help("Show help");
    parser.add_option("-f", "--filter").help("Filter benchmarks by the given regexp pattern");
    parser.add_option("-l", "--list").action("store_true").help("List all avaliable benchmarks");
    parser.add_option("-o", "--output").choices(&output[0], &output[3]).set_default(output[0]).help("Output format (console, csv, json). Default: %default");
    parser.add_option("-s", "--silent").action("store_true").help("Launch in silent mode");

    optparse::Values options = parser.parse_args(argc, argv);

    // Check for double initialization
    if (_init) {
        parser.error("Console launcher double initialization!");
    }

    // Print help
    if (options.get("help")) {
        parser.print_help();
        parser.exit();
    }

    // Setup console launcher parameters
    _list = options.get("list");
    _silent = options.get("silent");
    if (options.is_set("filter"))
        _filter = options["filter"];
    if (options.is_set("output"))
        _output = options["output"];

    // Update initialization flag
    _init = true;
}

void LauncherConsole::Launch()
{
    if (_list) {
        // List all suitable benchmarks
        std::regex matcher(_filter);
        for (auto benchmark : _benchmarks) {
            // Match benchmark name with the given pattern
            if (_filter.empty() || std::regex_match(benchmark->name(), matcher)) {
                std::cout << benchmark->name() << std::endl;
            }
        }
    }
    else {
        // Launch all suitable benchmarks
        Launcher::Launch(_filter);
    }
}

void LauncherConsole::Report()
{
    if (_output == "console") {
        ReporterConsole reporter(std::cout);
        Launcher::Report(reporter);
    }
    else if (_output == "csv") {
        ReporterCSV reporter(std::cout);
        Launcher::Report(reporter);
    }
    else if (_output == "json") {
        ReporterJSON reporter(std::cout);
        Launcher::Report(reporter);
    }
}

void LauncherConsole::onLaunching(int current, int total, const Benchmark& benchmark, const Context& context, int attempt)
{
    if (_silent)
        return;

    std::cerr << DARKGREY << "[" << std::setw(3) << (100 * current / total) << "%] " << GREY << "Launching " << LIGHTCYAN << benchmark.name() << context.to_string() << GREY << ". Attempt " << WHITE << attempt << GREY << "...";
}

void LauncherConsole::onLaunched(int current, int total, const Benchmark& benchmark, const Context& context, int attempt)
{
    if (_silent)
        return;

    std::cerr << LIGHTGREEN << "Done!" << std::endl;
}

} // namespace CppBenchmark
