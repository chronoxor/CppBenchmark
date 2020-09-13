/*!
    \file settings.h
    \brief Benchmark settings definition
    \author Ivan Shynkarenka
    \date 07.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_SETTINGS_H
#define CPPBENCHMARK_SETTINGS_H

#include <cstdint>
#include <functional>
#include <tuple>
#include <vector>

namespace CppBenchmark {

//! Benchmark settings
/*!
    Provides interface to all benchmark settings:
    - Independent benchmark attempts (default is 5)
    - Benchmark duration in seconds (default is 5)
    - Count of operations (default is 0)
    - Add count of running threads to the benchmark running plan
    - Add count of producers/consumers to the benchmark running plan
    - Add parameters (single, pair, triple) to the benchmark running plan

    All settings can be configured using fluent syntax.
*/
class Settings
{
    friend class Benchmark;
    friend class BenchmarkPC;
    friend class BenchmarkThreads;

public:
    //! Initialize settings with the default benchmark duration (5 seconds)
    Settings();
    //! Initialize settings with the given count of operations
    /*!
        \param operations - Count of operations
    */
    Settings(int64_t operations);
    Settings(const Settings&) = default;
    Settings(Settings&&) noexcept = default;
    ~Settings() = default;

    Settings& operator=(const Settings&) = default;
    Settings& operator=(Settings&&) noexcept = default;

    //! Get count of independent benchmark attempts
    int attempts() const noexcept { return _attempts; }
    //! Is benchmark running with infinite count of operations (until cancel)?
    bool infinite() const noexcept { return _infinite; }
    //! Get benchmark duration in milliseconds
    int64_t duration() const noexcept { return _duration; }
    //! Get count of operations
    int64_t operations() const noexcept { return _operations; }
    //! Get collection of independent threads counts in a benchmark plan
    const std::vector<int>& threads() const noexcept { return _threads; }
    //! Get collection of independent producers/consumers counts in a benchmark plan
    const std::vector<std::tuple<int, int>>& pc() const noexcept { return _pc; }
    //! Get collection of independent parameters in a benchmark plan
    const std::vector<std::tuple<int, int, int>>& params() const noexcept { return _params; }
    //! Get latency parameters
    const std::tuple<int64_t, int64_t, int>& latency() const noexcept { return _latency_params; }
    //! Get automatic latency update flag
    bool latency_auto() const noexcept { return _latency_auto; }

    //! Set independent benchmark attempts
    /*!
        Benchmark will be launched in several independent attempts. Best attempt with the lowest total execution time
        will be chosen as result one.

        \param attempts - Independent benchmark attempts (must be positive)
    */
    Settings& Attempts(int attempts);

    //! Set infinite benchmark operations flag
    /*!
        Benchmark will be launched in infinite operations loop until Context.Cancel() method is called.
    */
    Settings& Infinite();
    //! Set benchmark duration in seconds
    /*!
        \param duration - Benchmark duration in seconds (must be positive)
        \return Reference to the current settings instance
    */
    Settings& Duration(int64_t duration);
    //! Set count of operations
    /*!
        \param operations - Count of operations (must be positive)
        \return Reference to the current settings instance
    */
    Settings& Operations(int64_t operations);

    //! Add new threads count to the benchmark running plan
    /*!
        \param threads - Count of running threads (must be positive)
        \return Reference to the current settings instance
    */
    Settings& Threads(int threads);
    //! Add new threads range to the benchmark running plan
    /*!
        This method will iterate \a from - \a to and add each threads value to the benchmark running plan.

        \param from - From range of running threads (must be positive)
        \param to - To range of running threads (must be positive)
        \return Reference to the current settings instance
    */
    Settings& ThreadsRange(int from, int to);
    //! Add new threads range to the benchmark running plan using given selector function
    /*!
        This method will call selector function with \a from, \a to and current argument. Selector function may change
        current argument and return result value. If result value is bounds to \a from \a to range it will be add to
        the benchmark running plan. Otherwise selecting process will stop.

        \param from - From range of running threads (must be positive)
        \param to - To range of running threads (must be positive)
        \param selector - Selector function
        \return Reference to the current settings instance
    */
    Settings& ThreadsRange(int from, int to, const std::function<int (int, int, int&)>& selector);

    //! Add new producers/consumers count to the benchmark running plan
    /*!
        \param producers - Count of producers (must be positive)
        \param consumers - Count of consumers (must be positive)
        \return Reference to the current settings instance
    */
    Settings& PC(int producers, int consumers);
    //! Add new producers/consumers range to the benchmark running plan
    /*!
        This method will iterate \a producers_from, \a consumers_from - \a producers_to, \a consumers_to and add each
        producers/consumers pair to the benchmark running plan.

        \param producers_from - From range of producers (must be positive)
        \param producers_to - To range of producers (must be positive)
        \param consumers_from - From range of consumers (must be positive)
        \param consumers_to - To range of consumers (must be positive)
        \return Reference to the current settings instance
    */
    Settings& PCRange(int producers_from, int producers_to, int consumers_from, int consumers_to);
    //! Add new producers/consumers range to the benchmark running plan using given selector functions
    /*!
        This method will call selector functions with \a producers_from / \a consumers_from,
        \a producers_to / \a consumers_to and current arguments. Each selector function may change
        current argument and return result value. If result value is bounds to xxx_from - xxx_to range
        it will be add to the benchmark running plan. Otherwise selecting process will stop.

        \param producers_from - From range of producers (must be positive)
        \param producers_to - To range of producers (must be positive)
        \param producers_selector - Producers selector function
        \param consumers_from - From range of consumers (must be positive)
        \param consumers_to - To range of consumers (must be positive)
        \param consumers_selector - Consumers selector function
        \return Reference to the current settings instance
    */
    Settings& PCRange(int producers_from, int producers_to, const std::function<int (int, int, int&)>& producers_selector,
                      int consumers_from, int consumers_to, const std::function<int (int, int, int&)>& consumers_selector);

    //! Add new single parameter to the benchmark running plan
    /*!
        \param value - Single parameter (must be positive)
        \return Reference to the current settings instance
    */
    Settings& Param(int value);
    //! Add new single parameter range to the benchmark running plan
    /*!
        This method will iterate \a from - \a to and add each single parameter value to the benchmark running plan.

        \param from - From range of the single parameter (must be positive)
        \param to - To range of the single parameter (must be positive)
        \return Reference to the current settings instance
    */
    Settings& ParamRange(int from, int to);
    //! Add new single parameter range to the benchmark running plan using given selector function
    /*!
        This method will call selector function with \a from, \a to and current argument. Selector function may change
        current argument and return result value. If result value is bounds to \a from \a to range it will be add to
        the benchmark running plan. Otherwise selecting process will stop.

        \param from - From range of the single parameter (must be positive)
        \param to - To range of the single parameter (must be positive)
        \param selector - Selector function
        \return Reference to the current settings instance
    */
    Settings& ParamRange(int from, int to, const std::function<int (int, int, int&)>& selector);

    //! Add new parameters pair to the benchmark running plan
    /*!
        \param value1 - First parameter (must be positive)
        \param value2 - Second parameter (must be positive)
        \return Reference to the current settings instance
    */
    Settings& Pair(int value1, int value2);
    //! Add new parameters pairs range to the benchmark running plan
    /*!
        This method will iterate \a from1, \a from2 - \a to1, \a to2 and add each parameters pair to the benchmark
        running plan.

        \param from1 - From range of the first parameter (must be positive)
        \param to1 - To range of the first parameter (must be positive)
        \param from2 - From range of the second parameter (must be positive)
        \param to2 - To range of the second parameter (must be positive)
        \return Reference to the current settings instance
    */
    Settings& PairRange(int from1, int to1, int from2, int to2);
    //! Add new parameters pairs range to the benchmark running plan using given selector functions
    /*!
        This method will call selector functions with \a from1 / \a from2, \a to1 / \a to2 and current arguments.
        Each selector function may change current argument and return result value. If result value is bounds to
        xxx_from - xxx_to range it will be add to the benchmark running plan. Otherwise selecting process will stop.

        \param from1 - From range of the first parameter (must be positive)
        \param to1 - To range of the first parameter (must be positive)
        \param selector1 - Selector function for the first parameter
        \param from2 - From range of the second parameter (must be positive)
        \param to2 - To range of the second parameter (must be positive)
        \param selector2 - Selector function for the second parameter
        \return Reference to the current settings instance
    */
    Settings& PairRange(int from1, int to1, const std::function<int (int, int, int&)>& selector1,
                        int from2, int to2, const std::function<int (int, int, int&)>& selector2);

    //! Add new parameters triple to the benchmark running plan
    /*!
        \param value1 - First parameter (must be positive)
        \param value2 - Second parameter (must be positive)
        \param value3 - Third parameter (must be positive)
        \return Reference to the current settings instance
    */
    Settings& Triple(int value1, int value2, int value3);
    //! Add new parameters triple range to the benchmark running plan
    /*!
        This method will iterate \a from1, \a from2, \a from3 - \a to1, \a to2, \a to3 and add each parameters triple
        to the benchmark running plan.

        \param from1 - From range of the first parameter (must be positive)
        \param to1 - To range of the first parameter (must be positive)
        \param from2 - From range of the second parameter (must be positive)
        \param to2 - To range of the second parameter (must be positive)
        \param from3 - From range of the third parameter (must be positive)
        \param to3 - To range of the third parameter (must be positive)
        \return Reference to the current settings instance
    */
    Settings& TripleRange(int from1, int to1, int from2, int to2, int from3, int to3);
    //! Add new parameters triple range to the benchmark running plan using given selector functions
    /*!
        This method will call selector functions with \a from1 / \a from2 / \a from3, \a to1 / \a to2 / \a to3 and
        current arguments. Each selector function may change current argument and return result value. If result
        value is bounds to xxx_from - xxx_to range it will be add to the benchmark running plan. Otherwise selecting
        process will stop.

        \param from1 - From range of the first parameter (must be positive)
        \param to1 - To range of the first parameter (must be positive)
        \param selector1 - Selector function for the first parameter
        \param from2 - From range of the second parameter (must be positive)
        \param to2 - To range of the second parameter (must be positive)
        \param selector2 - Selector function for the second parameter
        \param from3 - From range of the third parameter (must be positive)
        \param to3 - To range of the third parameter (must be positive)
        \param selector3 - Selector function for the third parameter
        \return Reference to the current settings instance
    */
    Settings& TripleRange(int from1, int to1, const std::function<int (int, int, int&)>& selector1,
                          int from2, int to2, const std::function<int (int, int, int&)>& selector2,
                          int from3, int to3, const std::function<int (int, int, int&)>& selector3);

    //! Set latency histogram parameters
    /*!
        \param lowest - The smallest possible value to be put into the histogram
        \param highest - The largest possible value to be put into the histogram
        \param significant - Number of significant figures
        \param automatic - Automatic latency update (default is true)
        \return Reference to the current settings instance
    */
    Settings& Latency(int64_t lowest, int64_t highest, int significant, bool automatic = true);

private:
    int _attempts;
    bool _infinite;
    int64_t _duration;
    int64_t _operations;
    std::vector<int> _threads;
    std::vector<std::tuple<int, int>> _pc;
    std::vector<std::tuple<int, int, int>> _params;
    std::tuple<int64_t, int64_t, int> _latency_params;
    bool _latency_auto;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_H
