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
    Fixture() noexcept = default;
    Fixture(const Fixture&) noexcept = default;
    Fixture(Fixture&&) noexcept = default;
    virtual ~Fixture() noexcept = default;

    Fixture& operator=(const Fixture&) noexcept = default;
    Fixture& operator=(Fixture&&) noexcept = default;

protected:
    virtual void Initialize(Context& context) {}
    virtual void Cleanup(Context& context) {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_FIXTURE_H
