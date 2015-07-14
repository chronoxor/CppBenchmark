//
// Created by Ivan Shynkarenka on 02.07.2015.
//

#ifndef CPPBENCHMARK_PHASE_CORE_H
#define CPPBENCHMARK_PHASE_CORE_H

#include <limits>
#include <vector>

#include "phase_metrics.h"
#include "phase_scope.h"

namespace CppBenchmark {

class PhaseCore : public Phase
{
    friend class Benchmark;
    friend class Launcher;

public:
    explicit PhaseCore(const std::string& name) : _name(name)
    { _best._total_time = std::numeric_limits<int64_t>::max(); }
    PhaseCore(const PhaseCore&) = delete;
    PhaseCore(PhaseCore&&) = delete;
    virtual ~PhaseCore() = default;

    PhaseCore& operator=(const PhaseCore&) = delete;
    PhaseCore& operator=(PhaseCore&&) = delete;

    // Implementation of Phase
    const std::string& name() const override { return _name; }
    const PhaseMetrics& metrics() const override { return _metrics; }
    std::shared_ptr<Phase> StartPhase(const std::string& phase) override;
    void StopPhase() override;
    std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) override;

private:
    std::string _name;
    std::vector<std::shared_ptr<PhaseCore>> _child;
    PhaseMetrics _metrics;
    PhaseMetrics _best;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_CORE_H
