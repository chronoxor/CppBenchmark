//
// Created by Ivan Shynkarenka on 02.07.2015.
//

#ifndef CPPBENCHMARK_PHASE_CORE_H
#define CPPBENCHMARK_PHASE_CORE_H

#include <map>

#include "phase_metrics.h"
#include "phase_scope.h"

namespace CppBenchmark {

class PhaseCore : public Phase
{
    friend class Launcher;

public:
    explicit PhaseCore(const std::string& name) : _name(name) {}
    PhaseCore(const PhaseCore&) = delete;
    PhaseCore(PhaseCore&&) = delete;
    virtual ~PhaseCore() = default;

    PhaseCore& operator=(const PhaseCore&) = delete;
    PhaseCore& operator=(PhaseCore&&) = delete;

    // Implementation of Phase
    virtual const std::string& name() const { return _name; }
    virtual const PhaseMetrics& metrics() const { return _metrics; }
    virtual PhaseMetrics& metrics() { return _metrics; }
    virtual std::shared_ptr<Phase> Start(const std::string& phase);
    virtual void Stop();
    virtual std::shared_ptr<PhaseScope> Scope(const std::string& phase);

private:
    std::string _name;
    std::map<std::string, std::shared_ptr<PhaseCore>> _child;
    PhaseMetrics _metrics;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_CORE_H
