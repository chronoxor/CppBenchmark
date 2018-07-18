//
// Created by Ivan Shynkarenka on 18.07.2015
//

#include "benchmark/cppbenchmark.h"

#include <deque>
#include <list>
#include <vector>

template <typename T>
class ContainerFixture : public virtual CppBenchmark::Fixture
{
protected:
    T container;

    void Initialize(CppBenchmark::Context& context) override { container = T(); }
    void Cleanup(CppBenchmark::Context& context) override { container.clear(); }
};

BENCHMARK_FIXTURE(ContainerFixture<std::list<int>>, "std::list<int>.push_back()")
{
    container.push_back(0);
}

BENCHMARK_FIXTURE(ContainerFixture<std::vector<int>>, "std::vector<int>.push_back()")
{
    container.push_back(0);
}

BENCHMARK_FIXTURE(ContainerFixture<std::deque<int>>, "std::deque<int>.push_back()")
{
    container.push_back(0);
}

BENCHMARK_MAIN()
