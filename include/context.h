//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#ifndef CPPBENCHMARK_CONTEXT_H
#define CPPBENCHMARK_CONTEXT_H

#include <string>

#include "phase.h"

namespace CppBenchmark {

class Context : public Phase
{
    friend class Launcher;

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

    friend std::string to_string(const Context& instance);

    // Implementation of Phase
    const std::string& name() const override { return _phase.name(); }
    const PhaseMetrics& metrics() const override { return _phase.metrics(); }
    std::shared_ptr<Phase> StartPhase(const std::string& phase) override { return _phase.StartPhase(phase); }
    void StopPhase() override { _phase.StopPhase(); }
    std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) override { return _phase.ScopePhase(phase); }

private:
    Phase& _phase;
    int _x;
    int _y;
    int _z;

    Context(Phase& phase, int x, int y, int z) : _phase(phase), _x(x), _y(y), _z(z) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_CONTEXT_H
