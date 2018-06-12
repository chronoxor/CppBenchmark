/*!
    \file context.h
    \brief Benchmark running context definition
    \author Ivan Shynkarenka
    \date 07.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_CONTEXT_H
#define CPPBENCHMARK_CONTEXT_H

#include "benchmark/phase_core.h"

#include <atomic>

namespace CppBenchmark {

//! Benchmark running context
/*!
    Provides access to the current benchmark context information:
    - Benchmark parameters (x, y, z)
    - Benchmark metrics
    - Benchmark cancellation methods
    - Benchmark phase management methods
*/
class Context : public Phase
{
    friend class BenchmarkBase;
    friend class Benchmark;

public:
    Context() = delete;
    Context(const Context&) noexcept = default;
    Context(Context&&) noexcept = default;
    virtual ~Context() noexcept = default;

    Context& operator=(const Context&) noexcept = default;
    Context& operator=(Context&&) noexcept = default;

    //! Benchmark first parameter. Valid only if not negative!
    int x() const noexcept { return _x; }
    //! Benchmark second parameter. Valid only if not negative!
    int y() const noexcept { return _y; }
    //! Benchmark third parameter. Valid only if not negative!
    int z() const noexcept { return _z; }

    //! Benchmark mutable metrics
    /*!
        Benchmark mutable metrics can be used to increase operations count with PhaseMetrics.AddOperations()
        method, register processed items with PhaseMetrics.AddItems() method, register processed bytes with
        PhaseMetrics.AddBytes() method.
    */
    PhaseMetrics& metrics() noexcept { return *_metrics; }

    //! Is benchmark execution canceled?
    bool canceled() const noexcept { return *_canceled; }
    //! Cancel benchmark execution
    void Cancel() noexcept { *_canceled = true; }

    //! Get description of the current benchmark running context
    virtual std::string description() const;

    // Implementation of Phase
    const std::string& name() const noexcept override { return _current->name(); }
    const PhaseMetrics& metrics() const noexcept override { return _current->metrics(); }
    std::shared_ptr<Phase> StartPhase(const std::string& phase) override { return _current->StartPhase(phase); }
    std::shared_ptr<Phase> StartPhaseThreadSafe(const std::string& phase) override { return _current->StartPhaseThreadSafe(phase); }
    void StopPhase() override { _current->StopPhase(); }
    std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) override { return _current->ScopePhase(phase); }
    std::shared_ptr<PhaseScope> ScopePhaseThreadSafe(const std::string& phase) override { return _current->ScopePhaseThreadSafe(phase); }

protected:
    //! Benchmark first parameter. Valid only if not negative!
    int _x;
    //! Benchmark second parameter. Valid only if not negative!
    int _y;
    //! Benchmark third parameter. Valid only if not negative!
    int _z;
    //! Current benchmark phase
    PhaseCore* _current;
    //! Current benchmark metrics
    PhaseMetrics* _metrics;
    //! Benchmark canceled flag
    std::shared_ptr<std::atomic<bool>> _canceled;

    //! Create benchmark running context
    /*!
        \param x - Benchmark first parameter
        \param y - Benchmark second parameter
        \param z - Benchmark third parameter
    */
    Context(int x, int y, int z) noexcept
        : _x(x), _y(y), _z(z),
          _current(nullptr),
          _metrics(nullptr),
          _canceled(std::make_shared<std::atomic<bool>>(false))
    {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_H
