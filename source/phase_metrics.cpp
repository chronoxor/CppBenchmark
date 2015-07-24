//
// Created by Ivan Shynkarenka on 03.07.2015.
//

#include "phase_metrics.h"

namespace CppBenchmark {

void PhaseMetrics::StartPhase() noexcept
{
    _phase_time = std::chrono::high_resolution_clock::now();
}

void PhaseMetrics::StartIteration() noexcept
{
	AddIterations(1);
    _iteration_time = std::chrono::high_resolution_clock::now();
}

void PhaseMetrics::StopIteration() noexcept
{
    // Get iteration duration
    int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _iteration_time).count();

    // Update time counters
    if (duration < _min_time)
        _min_time = duration;
    if (duration > _max_time)
        _max_time = duration;
}

void PhaseMetrics::StopPhase() noexcept
{
    // Get phase duration
    int64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _phase_time).count();

    // Update total time counter
    _total_time += duration;
}

} // namespace CppBenchmark

