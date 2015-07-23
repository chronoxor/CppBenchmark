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
    friend class BenchmarkThreads;
    friend class Launcher;
    friend class PhaseSingleThread;
    friend class PhaseThreadSafe;

public:
    explicit PhaseCore(const std::string& name) : _name(name), _thread(System::CurrentThreadId())
    { _best._total_time = std::numeric_limits<int64_t>::max(); _worst._total_time = 0; }
    PhaseCore(const PhaseCore&) = delete;
    PhaseCore(PhaseCore&&) = delete;
    virtual ~PhaseCore() = default;

    PhaseCore& operator=(const PhaseCore&) = delete;
    PhaseCore& operator=(PhaseCore&&) = delete;

    PhaseMetrics& metrics() { return _metrics; }

    // Implementation of Phase
    const std::string& name() const override { return _name; }
    const PhaseMetrics& best() const override { return _best; }
    const PhaseMetrics& worst() const override { return _worst; }
    std::shared_ptr<Phase> StartPhase(const std::string& phase) override;
    std::shared_ptr<Phase> StartPhaseThreadSafe(const std::string& phase) override;
    void StopPhase() override;
    std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) override
    { return std::make_shared<PhaseScope>(StartPhase(phase)); }
    std::shared_ptr<PhaseScope> ScopePhaseThreadSafe(const std::string& phase) override
    { return std::make_shared<PhaseScope>(StartPhaseThreadSafe(phase)); }

protected:
    std::mutex _mutex;
    std::string _name;
    int _thread;
    std::vector<std::shared_ptr<PhaseCore>> _child;
    PhaseMetrics _metrics;
    PhaseMetrics _best;
    PhaseMetrics _worst;

    void StartIteration();
    void StopIteration();
    void Update();
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_CORE_H
