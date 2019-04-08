/*!
    \file benchmark.h
    \brief Benchmark definition
    \author Ivan Shynkarenka
    \date 06.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_BENCHMARK_H
#define CPPBENCHMARK_BENCHMARK_H

#include "benchmark/benchmark_base.h"
#include "benchmark/fixture.h"

namespace CppBenchmark {

//! Benchmark class
/*!
    Provides interface to perform single thread benchmark.
*/
class Benchmark : public BenchmarkBase, public virtual Fixture
{
public:
    //! Benchmark settings type
    typedef Settings TSettings;

    //! Default class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    template<typename... Types>
    explicit Benchmark(const std::string& name, Types... settings) : Benchmark(name, TSettings(settings...)) {}
    //! Alternative class constructor
    /*!
        \param name - Benchmark name
        \param settings - Benchmark settings
    */
    explicit Benchmark(const std::string& name, const TSettings& settings) : BenchmarkBase(name, settings) {}
    Benchmark(const Benchmark&) = delete;
    Benchmark(Benchmark&&) = delete;
    virtual ~Benchmark() = default;

    Benchmark& operator=(const Benchmark&) = delete;
    Benchmark& operator=(Benchmark&&) = delete;

protected:
    //! Benchmark run method
    /*!
        Should be implemented to provide code fragment to measure.

        \param context - Benchmark running context
    */
    virtual void Run(Context& context) = 0;

private:
    int CountLaunches() const override;
    void Launch(int& current, int total, LauncherHandler& handler) override;
};

/*! \example atomic.cpp Atomic operations benchmark */
/*! \example containers.cpp Containers push back benchmark */
/*! \example fpu.cpp FPU benchmark */
/*! \example fwrite.cpp File write benchmark */
/*! \example iterators.cpp Iterators benchmark */
/*! \example random.cpp Random distribution benchmark */
/*! \example sort.cpp Sort algorithms benchmark */

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_H
