/*!
    \file benchmark.h
    \brief Benchmark base class definition
    \author Ivan Shynkarenka
    \date 06.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_BENCHMARK_H
#define CPPBENCHMARK_BENCHMARK_H

#include "context.h"
#include "fixture.h"
#include "phase_core.h"
#include "settings.h"

namespace CppBenchmark {

class LauncherHandler;

//! Benchmark base class
/*!
    Provides interface to perform single thread benchmark.
*/
class Benchmark : public virtual Fixture
{
    friend class BenchmarkPC;
    friend class BenchmarkThreads;
    friend class Launcher;

public:
    //! Benchmark settings type
    typedef Settings TSettings;

    //! Default class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    explicit Benchmark(const std::string& name, const TSettings& settings = TSettings())
            : _launched(false),
              _name(name),
              _settings(settings)
    {}
    Benchmark(const Benchmark&) = delete;
    Benchmark(Benchmark&&) = delete;
    virtual ~Benchmark() = default;

    Benchmark& operator=(const Benchmark&) = delete;
    Benchmark& operator=(Benchmark&&) = delete;

    //! Get benchmark name
    const std::string& name() const { return _name; }

protected:
    //! Benchmark run method
    /*!
        Should be implemented to provide code fragment to measure.

        \param context - Benchmark running context
    */
    virtual void Run(Context& context) = 0;

private:
    bool _launched;
    std::string _name;
    Settings _settings;
    std::vector<std::shared_ptr<PhaseCore>> _phases;

    virtual int CountLaunches() const;
    virtual void Launch(int& current, int total, LauncherHandler& handler);

    void InitBenchmarkContext(Context& context);

    void UpdateBenchmarkMetrics();
    void UpdateBenchmarkMetrics(PhaseCore& phase);

    void UpdateBenchmarkThreads();
    void UpdateBenchmarkThreads(PhaseCore& phase);

    void UpdateBenchmarkNames();
    void UpdateBenchmarkNames(PhaseCore& phase, const std::string& name);
};

/*! \example atomic.cpp Atomic operations benchmark */
/*! \example containers.cpp Containers push back benchmark */
/*! \example fpu.cpp FPU benchmark */
/*! \example fwrite.cpp File write benchmark */
/*! \example iterators.cpp Iterators benchmark */
/*! \example random.cpp Random distribution benchmark */
/*! \example sort.cpp Sort algorithms benchmark */

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_H
