//
// Created by Ivan Shynkarenka on 03.07.2015.
//

#ifndef CPPBENCHMARK_PHASE_METRICS_H
#define CPPBENCHMARK_PHASE_METRICS_H

#include <chrono>
#include <cstdint>

namespace CppBenchmark {

class PhaseMetrics
{
    friend class PhaseCore;

public:
    PhaseMetrics() noexcept
            : _total_time(0),
              _total_iterations(0),
              _total_items(0),
              _total_bytes(0)
    {};
    PhaseMetrics(const PhaseMetrics&) noexcept = default;
    PhaseMetrics(PhaseMetrics&& instance) noexcept = default;
    ~PhaseMetrics() = default;

    PhaseMetrics& operator=(const PhaseMetrics&) noexcept = default;
    PhaseMetrics& operator=(PhaseMetrics&&) noexcept = default;

    int64_t total_time() const noexcept { return _total_time; }
    int64_t total_iterations() const noexcept { return _total_iterations; }
    int64_t total_items() const noexcept { return _total_items; }
    int64_t total_bytes() const noexcept { return _total_bytes; }

    int64_t time_per_iteration() const noexcept;
    int64_t iterations_per_second() const noexcept;
    int64_t items_per_second() const noexcept;
    int64_t bytes_per_second() const noexcept;

    void AddIterations(int64_t iterations) noexcept
    { _total_iterations += iterations; }
    void AddItems(int64_t items) noexcept
    { _total_items += items; }
    void AddBytes(int64_t bytes) noexcept
    { _total_bytes += bytes; }

private:
    int64_t _total_time;
    int64_t _total_iterations;
    int64_t _total_items;
    int64_t _total_bytes;

    std::chrono::high_resolution_clock::time_point _timestamp;

    void StartCollecting() noexcept
    { _timestamp = std::chrono::high_resolution_clock::now(); }
    void StopCollecting() noexcept
    { _total_time += std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - _timestamp).count(); }
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_METRICS_H
