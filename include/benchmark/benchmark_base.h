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
    //! Benchmark launched flag
    bool _launched;
    //! Benchmark name
    std::string _name;
    //! Benchmark settings
    Settings _settings;
    //! Benchmark phases
    std::vector<std::shared_ptr<PhaseCore>> _phases;

    //! Get the count of benchmark launches
    /*!
        \return Benchmark launches count
    */
    virtual int CountLaunches() const { return 1; }

    //! Get the count of benchmark launches
    /*!
        \param current - Current benchmark number
        \param total - Total benchmarks
        \param handler - Launcher handler
    */
    virtual void Launch(int& current, int total, LauncherHandler& handler) {}

    //! Initialize benchmark context
    /*!
        \param context - Benchmark context
    */
    void InitBenchmarkContext(Context& context);

    //! Update benchmark metrics for the given benchmark phases collection
    /*!
        \param phases - Benchmark phases collection
    */
    static void UpdateBenchmarkMetrics(std::vector<std::shared_ptr<PhaseCore>>& phases);
    //! Update benchmark metrics for the given benchmark phase
    /*!
        \param phase - Benchmark phase
    */
    static void UpdateBenchmarkMetrics(PhaseCore& phase);

    //! Update benchmark threads metrics for the given benchmark phases collection
    /*!
        \param phases - Benchmark phases collection
    */
    static void UpdateBenchmarkThreads(std::vector<std::shared_ptr<PhaseCore>>& phases);

    //! Update benchmark names for the given benchmark phases collection
    /*!
        \param phases - Benchmark phases collection
    */
    static void UpdateBenchmarkNames(std::vector<std::shared_ptr<PhaseCore>>& phases);
    //! Update benchmark names for the given benchmark phase
    /*!
        \param phase - Benchmark phase
        \param name - Benchmark name
    */
    static void UpdateBenchmarkNames(PhaseCore& phase, const std::string& name);

    //! Update benchmark operations for the given benchmark phases collection
    /*!
        \param phases - Benchmark phases collection
    */
    static void UpdateBenchmarkOperations(std::vector<std::shared_ptr<PhaseCore>>& phases);
    //! Update benchmark operations for the given benchmark phase
    /*!
        \param phase - Benchmark phase
    */
    static void UpdateBenchmarkOperations(PhaseCore& phase);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_BASE_H
