/*!
    \file phase.h
    \brief Benchmark phase base definition
    \author Ivan Shynkarenka
    \date 02.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_PHASE_H
#define CPPBENCHMARK_PHASE_H

#include "benchmark/phase_metrics.h"

#include <memory>

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
    Phase() noexcept = default;
    Phase(const Phase&) = default;
    Phase(Phase&&) noexcept = default;
    virtual ~Phase() = default;

    Phase& operator=(const Phase&) = default;
    Phase& operator=(Phase&&) noexcept = default;

    //! Get phase name
    virtual const std::string& name() const noexcept = 0;
    //! Get phase metrics
    virtual const PhaseMetrics& metrics() const noexcept = 0;

    //! Start a new sub-phase with a given name in a single-thread environment
    /*!
        This method will create or get existent sub-phase with a given name, start benchmark measurement and return it
        as a shared pointer. Please note the method is not thread-safe and should not be called in multi-thread
        environment!

        \param phase - Sub-phase name
        \return Shared pointer to the required benchmark sub-phase
    */
    virtual std::shared_ptr<Phase> StartPhase(const std::string& phase) = 0;

    //! Start a new sub-phase with a given name in a multi-thread environment
    /*!
        This method will create or get existent sub-phase with a given name, start benchmark measurement and return it
        as a shared pointer. Please note the method is thread-safe and might be called in multi-thread environment!

        \param phase - Sub-phase name
        \return Shared pointer to the thread safe benchmark sub-phase
    */
    virtual std::shared_ptr<Phase> StartPhaseThreadSafe(const std::string& phase) = 0;

    //! Stop measurement of the current phase
    virtual void StopPhase() = 0;

    //! Start a new sub-phase with a given name in a single-thread environment and wrap it in a PhaseScope
    /*!
        Please note the method is not thread-safe and should not be called in multi-thread environment!

        \param phase - Sub-phase name
        \return Shared pointer to the required benchmark sub-phase scope wrapper
    */
    virtual std::shared_ptr<PhaseScope> ScopePhase(const std::string& phase) = 0;

    //! Start a new sub-phase with a given name in a multi-thread environment and wrap it in a PhaseScope
    /*!
        Please note the method is thread-safe and might be called in multi-thread environment!

        \param phase - Sub-phase name
        \return Shared pointer to the required thread-safe benchmark sub-phase scope wrapper
    */
    virtual std::shared_ptr<PhaseScope> ScopePhaseThreadSafe(const std::string& phase) = 0;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_H
