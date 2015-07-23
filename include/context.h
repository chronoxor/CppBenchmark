//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#ifndef CPPBENCHMARK_CONTEXT_H
#define CPPBENCHMARK_CONTEXT_H

#include <string>

#include "phase_core.h"

namespace CppBenchmark {

class Context : public Phase
{
    friend class Benchmark;

public:
    Context() = delete;
    Context(const Context&) = default;
    Context(Context&&) = default;
    ~Context() = default;

    Context& operator=(const Context&) = default;
    Context& operator=(Context&&) = default;

    int x() const { return _x; }
    int y() const { return _y; }
    int z() const { return _z; }

    PhaseMetrics& metrics() { return *_metrics; }

    virtual std::string to_string() const;

    // Implementation of Phase
    const std::string& name() const override { return _current->name(); }
    const PhaseMetrics& best() const override { return _current->best(); }
    const PhaseMetrics& worst() const override { return _current->worst(); }
    std::shared_ptr<Phase> StartPhase(const std::string& phase) override { return _current->StartPhase(phase); }
    std::shared_ptr<Phase> StartPhaseThreadSafe(const std::string& phase) override { return _current->StartPhaseThreadSafe(phase); }
    void StopPhase() override { _current->StopPhase(); }
    std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) override { return _current->ScopePhase(phase); }
    std::shared_ptr<PhaseScope> ScopePhaseThreadSafe(const std::string& phase) override { return _current->ScopePhaseThreadSafe(phase); }

protected:
    int _x;
    int _y;
    int _z;
    PhaseCore* _current;
    PhaseMetrics* _metrics;

    Context(int x, int y, int z) : _x(x), _y(y), _z(z) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_H
