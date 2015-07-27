//
// Created by Ivan Shynkarenka on 07.07.2015.
//

#ifndef CPPBENCHMARK_SETTINGS_H
#define CPPBENCHMARK_SETTINGS_H

#include <functional>
#include <tuple>
#include <vector>

namespace CppBenchmark {

class Settings
{
    friend class Benchmark;
    friend class BenchmarkMPMC;
    friend class BenchmarkThreads;

public:
    Settings(int iterations = 1) : _attempts(5), _infinite(false), _iterations(iterations), _nanoseconds(0) {}
    Settings(const Settings&) = default;
    Settings(Settings&&) = default;
    ~Settings() = default;

    Settings& operator=(const Settings&) = default;
    Settings& operator=(Settings&&) = default;

    int attempts() const { return _attempts; }
    bool infinite() const { return _infinite; }
    int64_t iterations() const { return _iterations; }
    int64_t nanoseconds() const { return _nanoseconds; }
    const std::vector<int> threads() const { return _threads; }	
    const std::vector<std::tuple<int, int>> mpmc() const { return _mpmc; }	
    const std::vector<std::tuple<int, int, int>> params() const { return _params; }

    Settings& Attempts(int attempts);

    Settings& Infinite();
    Settings& Iterations(int64_t iterations);
    Settings& Nanoseconds(int64_t nanoseconds);
    Settings& Microseconds(int64_t microseconds);
    Settings& Milliseconds(int64_t milliseconds);
    Settings& Seconds(int64_t seconds);
    Settings& Minutes(int64_t minutes);
    Settings& Hours(int64_t hours);

    Settings& Threads(int threads);
    Settings& ThreadsRange(int from, int to);
    Settings& ThreadsRange(int from, int to, std::function<int (int, int, int)> selector);

    Settings& MPMC(int producers, int consumers);
    Settings& MPMCRange(int producers_from, int producers_to, int consumers_from, int consumers_to);
    Settings& MPMCRange(int producers_from, int producers_to, std::function<int (int, int, int)> producers_selector,
                        int consumers_from, int consumers_to, std::function<int (int, int, int)> consumers_selector);

    Settings& Param(int value);
    Settings& ParamRange(int from, int to);
    Settings& ParamRange(int from, int to, std::function<int (int, int, int)> selector);

    Settings& Pair(int value1, int value2);
    Settings& PairRange(int from1, int to1, int from2, int to2);
    Settings& PairRange(int from1, int to1, std::function<int (int, int, int)> selector1,
                        int from2, int to2, std::function<int (int, int, int)> selector2);

    Settings& Triple(int value1, int value2, int value3);
    Settings& TripleRange(int from1, int to1, int from2, int to2, int from3, int to3);
    Settings& TripleRange(int from1, int to1, std::function<int (int, int, int)> selector1,
                          int from2, int to2, std::function<int (int, int, int)> selector2,
                          int from3, int to3, std::function<int (int, int, int)> selector3);

private:
    int _attempts;
    bool _infinite;
    int64_t _iterations;
    int64_t _nanoseconds;
    std::vector<int> _threads;
    std::vector<std::tuple<int, int>> _mpmc;
    std::vector<std::tuple<int, int, int>> _params;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_H
