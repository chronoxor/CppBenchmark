//
// Created by ivan.shynkarenka on 16.07.2015.
//

#include <chrono>
#include <iostream>
#include <thread>

#include "macros.h"

using namespace CppBenchmark;

class TestBenchmark : public Benchmark
{
public:
    explicit TestBenchmark(const std::string& name, const Settings& settings = Settings::Default)
            : Benchmark(name, settings),
              _initializations(0),
              _runs(0),
              _cleanups(0)
    {
    }

    int initializations() const { return _initializations; }
    int runs() const { return _runs; }
    int cleanups() const { return _cleanups; }

protected:
    virtual void Initialize() { _initializations++; }
    virtual void Run(Context& context)
    {
        auto phase1 = context.StartPhase("Phase1");
        std::this_thread::yield();
        phase1->StopPhase();
        {
            auto phase2 = context.ScopePhase("Phase2");
            {
                auto phase21 = phase2->ScopePhase("Phase2.1");
                std::this_thread::sleep_for(std::chrono::milliseconds(context.x()));
            }
            {
                auto phase22 = phase2->ScopePhase("Phase2.2");
                std::this_thread::sleep_for(std::chrono::milliseconds(context.y()));
            }
        }
        auto phase3 = context.StartPhase("Phase3");
        std::this_thread::sleep_for(std::chrono::milliseconds(context.z()));
        phase3->StopPhase();

        _runs++;
    }
    virtual void Cleanup() { _cleanups++; }

private:
    int _initializations;
    int _runs;
    int _cleanups;
};

BENCHMARK("Test")
{
    auto phase1 = context.StartPhase("Phase1");
    std::this_thread::yield();
    phase1->StopPhase();
    {
        auto phase2 = context.ScopePhase("Phase2");
        {
            auto phase21 = phase2->ScopePhase("Phase2.1");
            std::this_thread::sleep_for(std::chrono::milliseconds(context.x()));
        }
        {
            auto phase22 = phase2->ScopePhase("Phase2.2");
            std::this_thread::sleep_for(std::chrono::milliseconds(context.y()));
        }
    }
    auto phase3 = context.StartPhase("Phase3");
    std::this_thread::sleep_for(std::chrono::milliseconds(context.z()));
    phase3->StopPhase();
}

BENCHMARK_MAIN()

/*
int main(int argc, char** argv)
{
    Settings settings = Settings().Attempts(5).Iterations(10).TripleRange(0, 1, 0, 1, 0, 1);
    std::shared_ptr<TestBenchmark> benchmark = std::make_shared<TestBenchmark>("Test", settings);

    LauncherConsole::GetInstance().Initialize(argc, argv);
    LauncherConsole::GetInstance().AddBenchmark(benchmark);
    LauncherConsole::GetInstance().Launch();
}
*/