/*!
    \file phase.h
    \brief Benchmark phase base class definition
    \author Ivan Shynkarenka
    \date 02.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_PHASE_H
#define CPPBENCHMARK_PHASE_H

#include <memory>
#include <string>

#include "phase_metrics.h"

namespace CppBenchmark {

class PhaseScope;

//! Benchmark phase base class
/*!
    Provides interface to start a new sub-phase, stop the current phase, create PhaseScope and access to the current
    phase name and metrics.
*/
class Phase
{
public:
    //! Get phase name
    virtual const std::string& name() const = 0;
    //! Get phase metrics
    virtual const PhaseMetrics& metrics() const = 0;

    //! Start a new sub-phase with a given name in a single-thread environment
    /*!
        This method will create or get existent sub-phase with a given name, start benchmark measurement and return it
        as a shared pointer. Please note the method is not thread-safe and should not be called in multi-thread
        environment!

        \param phase - Sub-phase name
    */
    virtual std::shared_ptr<Phase> StartPhase(const std::string& phase) = 0;

    //! Start a new sub-phase with a given name in a multi-thread environment
    /*!
        This method will create or get existent sub-phase with a given name, start benchmark measurement and return it
        as a shared pointer. Please note the method is thread-safe and might be called in multi-thread environment!

        \param phase - Sub-phase name
    */
    virtual std::shared_ptr<Phase> StartPhaseThreadSafe(const std::string& phase) = 0;

    //! Stop measurement of the current phase
    virtual void StopPhase() = 0;

    //! Start a new sub-phase with a given name in a single-thread environment and wrap it in a PhaseScope
    /*!
        Please note the method is not thread-safe and should not be called in multi-thread environment!

        \param phase - Sub-phase name
    */
    virtual std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) = 0;

    //! Start a new sub-phase with a given name in a multi-thread environment and wrap it in a PhaseScope
    /*!
        Please note the method is thread-safe and might be called in multi-thread environment!

        \param phase - Sub-phase name
    */
    virtual std::shared_ptr<PhaseScope> ScopePhaseThreadSafe(const std::string& phase) = 0;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_H
