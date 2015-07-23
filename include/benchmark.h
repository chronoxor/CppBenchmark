//
// Created by Ivan Shynkarenka on 01.07.2015.
//

#ifndef CPPBENCHMARK_BENCHMARK_H
#define CPPBENCHMARK_BENCHMARK_H

#include "context.h"
#include "fixture.h"
#include "phase_core.h"
#include "settings.h"

namespace CppBenchmark {

class LauncherHandler;

class Benchmark : public virtual Fixture
{
    friend class Launcher;

public:
    explicit Benchmark(const std::string& name, const Settings& settings = Settings())
            : _launched(false),
              _name(name),
              _settings(settings)
    {}
    Benchmark(const Benchmark&) = delete;
    Benchmark(Benchmark&&) = delete;
    virtual ~Benchmark() = default;

    Benchmark& operator=(const Benchmark&) = delete;
    Benchmark& operator=(Benchmark&&) = delete;

    bool is_launched() const { return _launched; }

    const std::string& name() const { return _name; }

protected:
    bool _launched;
    std::string _name;
    Settings _settings;
    std::vector<std::shared_ptr<PhaseCore>> _phases;

    virtual void Run(Context& context) = 0;

protected:
    virtual void Launch(LauncherHandler* handler);

    void InitBenchmarkContext(Context& context);

    void UpdateBenchmarkMetrics();
    void UpdateBenchmarkMetrics(PhaseCore& phase);

    void UpdateBenchmarkThreads();
    void UpdateBenchmarkThreads(PhaseCore& phase);

    void UpdateBenchmarkNames();
    void UpdateBenchmarkNames(PhaseCore& phase, const std::string& name);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_H
