/*!
    \file phase_core.h
    \brief Benchmark phase core definition
    \author Ivan Shynkarenka
    \date 02.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_PHASE_CORE_H
#define CPPBENCHMARK_PHASE_CORE_H

#include <limits>
#include <mutex>
#include <vector>

#include "phase_metrics.h"
#include "phase_scope.h"
#include "system.h"

namespace CppBenchmark {

//! Benchmark phase core
/*!
    Implementation of the Phase interface.
*/
class PhaseCore : public Phase
{
    friend class Benchmark;
    friend class BenchmarkPC;
    friend class BenchmarkThreads;
    friend class Executor;
    friend class Launcher;

public:
    //! Create a new benchmark phase core with a given name
    /*!
        \param name - Benchmark phase name
    */
    explicit PhaseCore(const std::string& name) : _name(name), _thread(System::CurrentThreadId())
    { _metrics_result._total_time = std::numeric_limits<int64_t>::max(); }
    PhaseCore(const PhaseCore&) = delete;
    PhaseCore(PhaseCore&&) = delete;
    virtual ~PhaseCore() = default;

    PhaseCore& operator=(const PhaseCore&) = delete;
    PhaseCore& operator=(PhaseCore&&) = delete;

    //! Current benchmark phase metrics
    PhaseMetrics& current() { return _metrics_current; }

    // Implementation of Phase
    const std::string& name() const override { return _name; }
    const PhaseMetrics& metrics() const override { return _metrics_result; }
    std::shared_ptr<Phase> StartPhase(const std::string& phase) override;
    std::shared_ptr<Phase> StartPhaseThreadSafe(const std::string& phase) override;
    void StopPhase() override { StopCollectingMetrics(); }
    std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) override
    { return std::make_shared<PhaseScope>(StartPhase(phase)); }
    std::shared_ptr<PhaseScope> ScopePhaseThreadSafe(const std::string& phase) override
    { return std::make_shared<PhaseScope>(StartPhaseThreadSafe(phase)); }

protected:
    //! Synchronization mutex
    std::mutex _mutex;
    //! Phase name
    std::string _name;
    //! Thread Id
    int _thread;
    //! Child phases container
    std::vector<std::shared_ptr<PhaseCore>> _child;
    //! Current phase metrics
    PhaseMetrics _metrics_current;
    //! Result phase metrics
    PhaseMetrics _metrics_result;

    //! Start collecting metrics in the current phase
    void StartCollectingMetrics() noexcept
    { _metrics_current.StartCollecting(); }
    //! Stop collecting metrics in the current phase
    void StopCollectingMetrics() noexcept
    { _metrics_current.StopCollecting(); }
    //! Merge phase metrics (current to result)
    void MergeMetrics()
    { _metrics_result.MergeMetrics(_metrics_current); }
    //! Merge metrics of the two phases
    void MergeMetrics(const PhaseCore& phase)
    { _metrics_result.MergeMetrics(phase._metrics_result); }
    //! Reset current phase metrics
    void ResetMetrics()
    { _metrics_current = PhaseMetrics(); }
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_CORE_H
