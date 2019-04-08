/*!
    \file launcher_console.h
    \brief Console launcher definition
    \author Ivan Shynkarenka
    \date 16.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_LAUNCHER_CONSOLE_H
#define CPPBENCHMARK_LAUNCHER_CONSOLE_H

#include "benchmark/launcher.h"

namespace CppBenchmark {

//! Console launcher
/*!
    Console launcher uses console output to show benchmarks running progress and sending reports to standard output.
    It can also parse command line arguments to provide additional launching options and different report formats.
*/
class LauncherConsole : public Launcher
{
public:
    LauncherConsole(const LauncherConsole&) = delete;
    LauncherConsole(LauncherConsole&&) = delete;
    virtual ~LauncherConsole() = default;

    LauncherConsole& operator=(const LauncherConsole&) = delete;
    LauncherConsole& operator=(LauncherConsole&&) = delete;

    //! Get singleton instance
    static LauncherConsole& GetInstance()
    { static LauncherConsole instance; return instance; }

    //! Initialized console launcher
    /*!
        This method should be called once from main() function to initialize console launcher with command line
        arguments!

        \see BENCHMARK_MAIN()

        \param argc - Arguments count
        \param argv - Arguments values
    */
    void Initialize(const int argc, char const* const* const argv);
    //! Execute benchmarks and show progress in console
    void Execute();
    //! Report benchmarks results in console
    void Report() const;

protected:
    // Implementation of LauncherHanlder
    void onLaunching(int current, int total, const BenchmarkBase& benchmark, const Context& context, int attempt) override;
    void onLaunched(int current, int total, const BenchmarkBase& benchmark, const Context& context, int attempt) override;

private:
    bool _init;
    bool _list;
    bool _quiet;
    int32_t _histograms;
    std::string _filter;
    std::string _output;

    LauncherConsole() : _init(false), _list(false), _quiet(false), _histograms(0), _filter(""), _output("console") {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_LAUNCHER_CONSOLE_H
