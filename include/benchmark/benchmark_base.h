/*!
    \file benchmark_base.h
    \brief Benchmark base definition
    \author Ivan Shynkarenka
    \date 06.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_BENCHMARK_BASE_H
#define CPPBENCHMARK_BENCHMARK_BASE_H

#include "benchmark/context.h"
#include "benchmark/phase_core.h"
#include "benchmark/settings.h"

namespace CppBenchmark {

class LauncherHandler;

//! Benchmark base class
/*!
    Provides base interface for all benchmarks.
*/
class BenchmarkBase
{
    friend class Executor;
    friend class Launcher;

public:
    //! Default class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    template<typename... Types>
    explicit BenchmarkBase(const std::string& name, Types... settings)
        : BenchmarkBase(name, Settings(settings...))
    {}
    //! Alternative class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    explicit BenchmarkBase(const std::string& name, const Settings& settings)
        : _launched(false), _name(name), _settings(settings)
    {}
    BenchmarkBase(const BenchmarkBase&) = delete;
    BenchmarkBase(BenchmarkBase&&) = delete;
    virtual ~BenchmarkBase() = default;

    BenchmarkBase& operator=(const BenchmarkBase&) = delete;
    BenchmarkBase& operator=(BenchmarkBase&&) = delete;

    //! Get benchmark name
    const std::string& name() const { return _name; }
    //! Get benchmark settings
    const Settings& settings() const { return _settings; }

protected:
    bool _launched;
    std::string _name;
    Settings _settings;
    std::vector<std::shared_ptr<PhaseCore>> _phases;

    virtual int CountLaunches() const { return 1; }
    virtual void Launch(int& current, int total, LauncherHandler& handler) {}

    void InitBenchmarkContext(Context& context);

    static void UpdateBenchmarkMetrics(std::vector<std::shared_ptr<PhaseCore>>& phases);
    static void UpdateBenchmarkMetrics(PhaseCore& phase);
    static void UpdateBenchmarkThreads(std::vector<std::shared_ptr<PhaseCore>>& phases);
    static void UpdateBenchmarkNames(std::vector<std::shared_ptr<PhaseCore>>& phases);
    static void UpdateBenchmarkNames(PhaseCore& phase, const std::string& name);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_BASE_H
