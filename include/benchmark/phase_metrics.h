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
#include <string>

namespace CppBenchmark {

//! Benchmark phase metrics
/*!
    Provides interface of the phase metrics to collect benchmark running statistics:
    - Average time of the phase execution
    - Minimal time of the phase execution
    - Maximal time of the phase execution
    - Total time of the phase execution
    - Total operations made in the phase
    - Total items processed in the phase
    - Total bytes processed in the phase

    If the phase metrics is accessed from benchmark running Context you can update some metrics values:
    - increase operations count with AddOperations() method
    - register processed items with AddItems() method
    - register processed bytes with AddBytes() method
    - set custom integer/string values by name
*/
class PhaseMetrics
{
    friend class PhaseCore;

public:
    //! Default constructor
    PhaseMetrics();
    PhaseMetrics(const PhaseMetrics&) = default;
    PhaseMetrics(PhaseMetrics&&) noexcept = default;
    ~PhaseMetrics();

    PhaseMetrics& operator=(const PhaseMetrics&) = default;
    PhaseMetrics& operator=(PhaseMetrics&&) noexcept = default;

    //! Is metrics contains latency values?
    bool latency() const noexcept;
    //! Get latency minimal value of the phase execution
    int64_t min_latency() const noexcept;
    //! Get latency maximal value of the phase execution
    int64_t max_latency() const noexcept;
    //! Get latency mean value of the phase execution
    double mean_latency() const noexcept;
    //! Get latency standard deviation of the phase execution
    double stdv_latency() const noexcept;

    //! Get average time of the phase execution
    int64_t avg_time() const noexcept;
    //! Get minimal time of the phase execution
    int64_t min_time() const noexcept;
    //! Get maximal time of the phase execution
    int64_t max_time() const noexcept;

    //! Get total time of the phase execution
    int64_t total_time() const noexcept { return _total_time; }
    //! Get total operations made in the phase
    int64_t total_operations() const noexcept { return _total_operations; }
    //! Get total items processed in the phase
    int64_t total_items() const noexcept { return _total_items; }
    //! Get total bytes processed in the phase
    int64_t total_bytes() const noexcept { return _total_bytes; }

    //! Get operations throughput (operations / second)
    int64_t operations_per_second() const noexcept;
    //! Get items throughput (items / second)
    int64_t items_per_second() const noexcept;
    //! Get data throughput (bytes / second)
    int64_t bytes_per_second() const noexcept;

    //! Get custom integers map
    const std::map<std::string, int>& custom_int() const noexcept { return _custom_int; }
    //! Get custom unsigned integers map
    const std::map<std::string, unsigned>& custom_uint() const noexcept { return _custom_uint; }
    //! Get custom integers 64-bit map
    const std::map<std::string, int64_t>& custom_int64() const noexcept { return _custom_int64; }
    //! Get custom unsigned integers 64-bit map
    const std::map<std::string, uint64_t>& custom_uint64() const noexcept { return _custom_uint64; }
    //! Get custom float map
    const std::map<std::string, float>& custom_flt() const noexcept { return _custom_flt; }
    //! Get custom doubles map
    const std::map<std::string, double>& custom_dbl() const noexcept { return _custom_dbl; }
    //! Get custom strings map
    const std::map<std::string, std::string>& custom_str() const noexcept { return _custom_str; }

    int threads() const noexcept { return _threads; }

    //! Increase operations count of the current phase
    /*!
        \param operations - Operations count
    */
    void AddOperations(int64_t operations) noexcept
    { _total_operations += operations; }
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
    void SetCustom(const std::string& name, int value)
    { _custom_int[name] = value; }
    //! Set custom unsigned integer value
    /*!
        \param name - Name
        \param value - Value
    */
    void SetCustom(const std::string& name, unsigned value)
    { _custom_uint[name] = value; }
    //! Set custom integer 64-bit value
    /*!
        \param name - Name
        \param value - Value
    */
    void SetCustom(const std::string& name, int64_t value)
    { _custom_int64[name] = value; }
    //! Set custom unsigned integer 64-bit value
    /*!
        \param name - Name
        \param value - Value
    */
    void SetCustom(const std::string& name, uint64_t value)
    { _custom_uint64[name] = value; }
    //! Set custom float value
    /*!
        \param name - Name
        \param value - Value
    */
    void SetCustom(const std::string& name, float value)
    { _custom_flt[name] = value; }
    //! Set custom double value
    /*!
        \param name - Name
        \param value - Value
    */
    void SetCustom(const std::string& name, double value)
    { _custom_dbl[name] = value; }
    //! Set custom string value
    /*!
        \param name - Name
        \param value - Value
    */
    void SetCustom(const std::string& name, const std::string& value)
    { _custom_str[name].assign(value); }

    //! Set threads value
    /*!
        \param threads - Threads
    */
    void SetThreads(int threads)
    { _threads = threads; }

    //! Add latency value of the current phase
    /*!
        \param latency - Latency value
    */
    void AddLatency(int64_t latency) noexcept;

private:
    void* _histogram;
    int64_t _min_time;
    int64_t _max_time;
    int64_t _total_time;
    int64_t _total_operations;
    int64_t _total_items;
    int64_t _total_bytes;
    std::map<std::string, int> _custom_int;
    std::map<std::string, unsigned> _custom_uint;
    std::map<std::string, int64_t> _custom_int64;
    std::map<std::string, uint64_t> _custom_uint64;
    std::map<std::string, float> _custom_flt;
    std::map<std::string, double> _custom_dbl;
    std::map<std::string, std::string> _custom_str;

    int64_t _iterstamp;
    int64_t _timestamp;

    int _threads;

    void InitLatencyHistogram(const std::tuple<int64_t, int64_t, int>& latency) noexcept;
    void PrintLatencyHistogram(FILE* file, int32_t resolution) const noexcept;
    void FreeLatencyHistogram() noexcept;

    void StartCollecting() noexcept;
    void StopCollecting() noexcept;

    void MergeMetrics(PhaseMetrics& metrics);
    void ResetMetrics() noexcept;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_PHASE_METRICS_H
