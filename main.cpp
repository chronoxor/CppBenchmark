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
    explicit TestBenchmark(const std::string& name, const Settings& settings = Settings())
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

BENCHMARK("Test", Settings().Param(0).Param(2).Param(4))
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

struct abc
{
public:
    abc() { std::cout << "abc::abc()" << std::endl; i=100; }
    ~abc() { std::cout << "abc::~abc()" << std::endl; }

protected:
    int i;
};

BENCHMARK_FIXTURE(abc, "Test", Settings().Param(100))
{
    std::cout << "abc::i = " << i << std::endl;
}

BENCHMARK_CLASS(TestBenchmark, "Test", Settings().Param(200))

BENCHMARK_MAIN()
