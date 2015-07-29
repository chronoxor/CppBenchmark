//
// Created by Ivan Shynkarenka on 16.07.2015.
//

#ifndef CPPBENCHMARK_LAUNCHER_CONSOLE_H
#define CPPBENCHMARK_LAUNCHER_CONSOLE_H

#include "launcher.h"

namespace CppBenchmark {

class LauncherConsole : public Launcher
{
public:
    LauncherConsole(const Benchmark&) = delete;
    LauncherConsole(Benchmark&&) = delete;
    virtual ~LauncherConsole() = default;

    LauncherConsole& operator=(const LauncherConsole&) = delete;
    LauncherConsole& operator=(LauncherConsole&&) = delete;

    void Initialize(int argc, char** argv);
    void Launch();
    void Report();

    static LauncherConsole& GetInstance()
    { static LauncherConsole instance; return instance; }

protected:
    // Implementation of LauncherHanlder
    void onLaunching(int current, int total, const Benchmark& benchmark, const Context& context, int attempt) override;
    void onLaunched(int current, int total, const Benchmark& benchmark, const Context& context, int attempt) override;

private:
    bool _init;
    bool _list;
    bool _silent;
    std::string _filter;
    std::string _output;

    LauncherConsole() : _init(false), _list(false), _silent(false), _filter(""), _output("console") {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_LAUNCHER_CONSOLE_H
