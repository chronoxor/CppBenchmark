//
// Created by Ivan Shynkarenka on 02.07.2015.
//

#ifndef CPPBENCHMARK_PHASE_H
#define CPPBENCHMARK_PHASE_H

#include <memory>
#include <string>

#include "phase_metrics.h"

namespace CppBenchmark {

class PhaseScope;

class Phase
{
public:
    virtual const std::string& name() const = 0;
    virtual const PhaseMetrics& metrics() const = 0;

    virtual PhaseMetrics& metrics() = 0;

    virtual std::shared_ptr<Phase> StartPhase(const std::string& phase) = 0;
    virtual void StopPhase() = 0;

    virtual std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) = 0;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_H
