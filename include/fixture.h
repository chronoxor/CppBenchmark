//
// Created by Ivan Shynkarenka on 18.07.2015.
//

#ifndef CPPBENCHMARK_FIXTURE_H
#define CPPBENCHMARK_FIXTURE_H

#include "context.h"

namespace CppBenchmark {

class Fixture
{
public:
    Fixture() = default;
    Fixture(const Fixture&) = default;
    Fixture(Fixture&&) = default;
    virtual ~Fixture() = default;

    Fixture& operator=(const Fixture&) = default;
    Fixture& operator=(Fixture&&) = default;

protected:
    virtual void Initialize(Context& context) {}
    virtual void Cleanup(Context& context) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_H
