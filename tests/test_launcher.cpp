//
// Created by Ivan Shynkarenka on 14.07.2015.
//

#include "catch.hpp"

#include <chrono>
#include <thread>

#include "launcher.h"
#include "reporter_console.h"
#include "reporter_csv.h"
#include "reporter_json.h"

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
    virtual void Run(const Context& context)
    {
        auto phase1 = StartPhase("Phase1");
        std::this_thread::yield();
        phase1->StopPhase();
        {
            auto phase2 = ScopePhase("Phase2");
            {
                auto phase21 = phase2->ScopePhase("Phase2.1");
                std::this_thread::sleep_for(std::chrono::milliseconds(context.x()));
            }
            {
                auto phase22 = phase2->ScopePhase("Phase2.2");
                std::this_thread::sleep_for(std::chrono::milliseconds(context.y()));
            }
        }
        auto phase3 = StartPhase("Phase3");
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

class TestLauncher : public Launcher
{
public:
    TestLauncher() : _launching(0), _launched(0) {}

    int launching() { return _launching; }
    int launched() { return _launched; }

protected:
    void onLaunching(const Benchmark& benchmark, const Context& context, int attempt) override { _launching++; }
    void onLaunched(const Benchmark& benchmark, const Context& context, int attempt) override { _launched++; }

private:
    int _launching;
    int _launched;
};

TEST_CASE("Launcher complex test", "[CppBenchmark][Launcher][Reporter]")
{
    // Prepare benchmark
    Settings settings = Settings().Attempts(5).Iterations(10).TripleRange(0, 1, 0, 1, 0, 1);
    std::shared_ptr<TestBenchmark> benchmark = std::make_shared<TestBenchmark>("Test", settings);

    // Prepare reporters
    std::ostringstream stream_console;
    std::shared_ptr<ReporterConsole> reporter_console = std::make_shared<ReporterConsole>(stream_console);
    std::ostringstream stream_csv;
    std::shared_ptr<ReporterCSV> reporter_csv = std::make_shared<ReporterCSV>(stream_csv);
    std::ostringstream stream_json;
    std::shared_ptr<ReporterJSON> reporter_json = std::make_shared<ReporterJSON>(stream_json);

    // Prepare launcher
    TestLauncher launcher;
    launcher.AddBenchmark(benchmark);
    launcher.AddReporter(reporter_console);
    launcher.AddReporter(reporter_csv);
    launcher.AddReporter(reporter_json);

    // Launch
    launcher.Launch(".es.");

    // Collect benchmark reports
    std::string report_console = stream_console.str();
    std::string report_csv = stream_csv.str();
    std::string report_json = stream_json.str();

    // Test benchmark reports
    REQUIRE(report_console.size() > 0);
    REQUIRE(report_csv.size() > 0);
    REQUIRE(report_json.size() > 0);

    std::cout << report_console << std::endl;
    std::cout << report_csv << std::endl;
    std::cout << report_json << std::endl;

    // Test benchmark state
    REQUIRE(benchmark->initializations() == settings.attempts());
    REQUIRE(benchmark->runs() == (settings.params().size() * settings.attempts() * settings.iterations()));
    REQUIRE(benchmark->cleanups() == benchmark->initializations());

    // Test launcher state
    REQUIRE(launcher.launching() == (settings.params().size() * settings.attempts()));
    REQUIRE(launcher.launching() == launcher.launched());
}
