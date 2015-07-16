//
// Created by Ivan Shynkarenka on 01.07.2015.
//

#ifndef CPPBENCHMARK_BENCHMARK_H
#define CPPBENCHMARK_BENCHMARK_H

#include "context.h"
#include "phase_core.h"
#include "settings.h"

namespace CppBenchmark {

class Benchmark
{
    friend class Launcher;

public:
    explicit Benchmark(const std::string& name, const Settings& settings = Settings()) : _name(name), _settings(settings) {}
    Benchmark(const Benchmark&) = delete;
    Benchmark(Benchmark&&) = delete;
    virtual ~Benchmark() = default;

    Benchmark& operator=(const Benchmark&) = delete;
    Benchmark& operator=(Benchmark&&) = delete;

    const std::string& name() const { return _name; }

protected:
    virtual void Initialize() {}
    virtual void Run(Context& context) = 0;
    virtual void Cleanup() {}

private:
    std::string _name;
    Settings _settings;
    std::shared_ptr<PhaseCore> _current;
    std::vector<std::shared_ptr<PhaseCore>> _phases;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_H
