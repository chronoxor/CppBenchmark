//
// Created by Ivan Shynkarenka on 02.07.2015.
//

#ifndef CPPBENCHMARK_PHASE_CORE_H
#define CPPBENCHMARK_PHASE_CORE_H

#include <limits>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "phase_metrics.h"
#include "phase_scope.h"
#include "system.h"

namespace CppBenchmark {

class PhaseCore : public Phase
{
    friend class Benchmark;
    friend class BenchmarkPC;
    friend class BenchmarkThreads;
    friend class Launcher;

public:
    explicit PhaseCore(const std::string& name) : _name(name), _thread(System::CurrentThreadId())
    { _metrics_result._total_time = std::numeric_limits<int64_t>::max(); }
    PhaseCore(const PhaseCore&) = delete;
    PhaseCore(PhaseCore&&) = delete;
    virtual ~PhaseCore() = default;

    PhaseCore& operator=(const PhaseCore&) = delete;
    PhaseCore& operator=(PhaseCore&&) = delete;

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
    std::mutex _mutex;
    std::string _name;
    int _thread;
    std::vector<std::shared_ptr<PhaseCore>> _child;
    PhaseMetrics _metrics_current;
    PhaseMetrics _metrics_result;

    void StartCollectingMetrics() noexcept
    { _metrics_current.StartCollecting(); }
    void StopCollectingMetrics() noexcept
    { _metrics_current.StopCollecting(); }
    void MergeMetrics() noexcept
    { _metrics_result.MergeMetrics(_metrics_current); }
    void MergeMetrics(const PhaseCore& phase) noexcept
    { _metrics_result.MergeMetrics(phase._metrics_result); }
    void ResetMetrics() noexcept
    { _metrics_current = PhaseMetrics(); }
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_CORE_H
