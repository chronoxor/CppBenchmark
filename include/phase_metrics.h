/*!
    \file phase_metrics.h
    \brief Benchmark phase metrics definition
    \author Ivan Shynkarenka
    \date 03.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_PHASE_METRICS_H
#define CPPBENCHMARK_PHASE_METRICS_H

#include <cstdint>
#include <limits>
#include <map>

namespace CppBenchmark {

//! Benchmark phase metrics
/*!
    Provides interface of the phase metrics to collect benchmark running statistics:
    - Average time of the phase execution
    - Minimal time of the phase execution
    - Maximal time of the phase execution
    - Total time of the phase execution
    - Total iterations made in the phase
    - Total items processed in the phase
    - Total bytes processed in the phase

    If the phase metrics is accessed from benchmark running Context you can update some metrics values:
    - increase iterations count with AddIterations() method
    - register processed items with AddItems() method
    - register processed bytes with AddBytes() method
    - set custom integer/string values by name
*/
class PhaseMetrics
{
    friend class PhaseCore;

public:
    //! Default constructor
    PhaseMetrics()
            : _min_time(std::numeric_limits<int64_t>::max()),
              _max_time(std::numeric_limits<int64_t>::min()),
              _total_time(0),
              _total_iterations(0),
              _total_items(0),
              _total_bytes(0)
    {};
    PhaseMetrics(const PhaseMetrics&) = default;
    PhaseMetrics(PhaseMetrics&& instance) = default;
    ~PhaseMetrics() = default;

    PhaseMetrics& operator=(const PhaseMetrics&) = default;
    PhaseMetrics& operator=(PhaseMetrics&&) = default;

    //! Get average time of the phase execution
    int64_t avg_time() const noexcept;
    //! Get minimal time of the phase execution
    int64_t min_time() const noexcept;
    //! Get maximal time of the phase execution
    int64_t max_time() const noexcept;

    //! Get total time of the phase execution
    int64_t total_time() const noexcept { return _total_time; }
    //! Get total iterations made in the phase
    int64_t total_iterations() const noexcept { return _total_iterations; }
    //! Get total items processed in the phase
    int64_t total_items() const noexcept { return _total_items; }
    //! Get total bytes processed in the phase
    int64_t total_bytes() const noexcept { return _total_bytes; }

    //! Get iterations throughput (iterations / second)
    int64_t iterations_per_second() const noexcept;
    //! Get items throughput (items / second)
    int64_t items_per_second() const noexcept;
    //! Get data throughput (bytes / second)
    int64_t bytes_per_second() const noexcept;

    //! Get custom integers map
    const std::map<std::string, int64_t>& custom_int() const noexcept { return _custom_int; }
    //! Get custom strings map
    const std::map<std::string, std::string>& custom_str() const noexcept { return _custom_str; }

    //! Increase iterations count of the current phase
    /*!
        \param iterations - Iterations count
    */
    void AddIterations(int64_t iterations) noexcept
    { _total_iterations += iterations; }
    //! Register processed items in the current phase
    /*!
        \param items - Items count
    */
    void AddItems(int64_t items) noexcept
    { _total_items += items; }
    //! Register processed bytes in the current phase
    /*!
        \param bytes - Bytes count
    */
    void AddBytes(int64_t bytes) noexcept
    { _total_bytes += bytes; }

    //! Set custom integer value
    /*!
        \param name - Name
        \param value - Value
    */
    void SetCustom(const std::string& name, int64_t value)
    { _custom_int[name] = value; }
    //! Set custom string value
    /*!
        \param name - Name
        \param value - Value
    */
    void SetCustom(const std::string& name, const std::string& value)
    { _custom_str[name] = value; }

private:
    int64_t _min_time;
    int64_t _max_time;
    int64_t _total_time;
    int64_t _total_iterations;
    int64_t _total_items;
    int64_t _total_bytes;
    std::map<std::string, int64_t> _custom_int;
    std::map<std::string, std::string> _custom_str;

    int64_t _iterstamp;
    int64_t _timestamp;

    void StartCollecting() noexcept;
    void StopCollecting() noexcept;

    void MergeMetrics(const PhaseMetrics& metrics);
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_METRICS_H
