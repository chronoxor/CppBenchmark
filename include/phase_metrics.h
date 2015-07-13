//
// Created by Ivan Shynkarenka on 03.07.2015.
//

#ifndef CPPBENCHMARK_PHASE_METRICS_H
#define CPPBENCHMARK_PHASE_METRICS_H

#include <chrono>
#include <cstdint>
#include <limits>

namespace CppBenchmark {

class PhaseMetrics
{
    friend class Benchmark;
    friend class PhaseCore;

public:
    PhaseMetrics() noexcept
            : _min_time(std::numeric_limits<int64_t>::max()),
              _max_time(std::numeric_limits<int64_t>::min()),
              _total_time(0),
              _total_iterations(0),
              _total_items(0),
              _total_bytes(0),
              _is_started(false)
    {};
    PhaseMetrics(const PhaseMetrics&) noexcept = default;
    PhaseMetrics(PhaseMetrics&& instance) noexcept = default;
    ~PhaseMetrics() = default;

    PhaseMetrics& operator=(const PhaseMetrics&) noexcept = default;
    PhaseMetrics& operator=(PhaseMetrics&&) noexcept = default;

    int64_t min_time() const { return _min_time; }
    int64_t max_time() const { return _max_time; }
    int64_t total_time() const { return _total_time; }
    int64_t total_iterations() const { return _total_iterations; }
    int64_t total_items() const { return _total_items; }
    int64_t total_bytes() const { return _total_bytes; }

    int64_t avg_time() const { return (_total_iterations > 0) ? (_total_time / _total_iterations) : 0; }

    double iterations_per_second() const { return (_total_time > 0) ? (((double)_total_iterations / (double)_total_time) * 1000000000.0) : 0.0; }
    double items_per_second() const { return (_total_time > 0) ? (((double)_total_items / (double)_total_time) * 1000000000.0) : 0.0; }
    double bytes_per_second() const { return (_total_time > 0) ? (((double)_total_bytes / (double)_total_time) * 1000000000.0) : 0.0; }

    void AddItems(int64_t items) { if (_is_started) _total_items += items; }
    void AddBytes(int64_t bytes) { if (_is_started) _total_bytes += bytes; }

private:
    int64_t _min_time;
    int64_t _max_time;
    int64_t _total_time;
    int64_t _total_iterations;
    int64_t _total_items;
    int64_t _total_bytes;

    bool _is_started;
    std::chrono::high_resolution_clock::time_point _start_time;
    std::chrono::high_resolution_clock::time_point _stop_time;

    void StartIteration();
    void StopIteration();
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_METRICS_H
