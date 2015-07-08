//
// Created by Ivan Shynkarenka on 01.07.2015.
//

#ifndef CPPBENCHMARK_BENCHMARK_H
#define CPPBENCHMARK_BENCHMARK_H

#include "context.h"
#include "phase_core.h"
#include "settings.h"

namespace CppBenchmark {

class Benchmark : public Phase
{
    friend class Launcher;

public:
    explicit Benchmark(const std::string& name, const Settings& settings = Settings::Default)
            : _root(name),
              _settings(settings)
    {}
    Benchmark(const Benchmark&) = delete;
    Benchmark(Benchmark&&) = delete;
    virtual ~Benchmark() = default;

    Benchmark& operator=(const Benchmark&) = delete;
    Benchmark& operator=(Benchmark&&) = delete;

    // Implementation of Phase
    virtual const std::string& name() const
    { return _root.name(); }
    virtual const PhaseMetrics& metrics() const
    { return _root.metrics(); }
    virtual PhaseMetrics& metrics()
    { return _root.metrics(); }
    virtual std::shared_ptr<Phase> Start(const std::string& phase)
    { return _root.Start(phase); }
    virtual void Stop()
    { _root.Stop(); }
    virtual std::shared_ptr<PhaseScope> Scope(const std::string& phase)
    { return _root.Scope(phase); }

protected:
    virtual void Initialize() {}
    virtual void Run(const Context& context) = 0;
    virtual void Cleanup() {}

private:
    PhaseCore _root;
    Settings _settings;

    void Launch(std::function<void (const Benchmark&, const Context&, int)> onLaunching = [](const Benchmark&, const Context&, int){},
                std::function<void (const Benchmark&, const Context&, int)> onLaunched = [](const Benchmark&, const Context&, int){});
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_H
