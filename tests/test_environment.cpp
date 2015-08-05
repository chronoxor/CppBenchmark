//
// Created by Ivan Shynkarenka on 09.07.2015.
//

#include "catch.hpp"

#include "environment.h"

using namespace CppBenchmark;

TEST_CASE("Environment information", "[CppBenchmark][Environment]")
{
    REQUIRE((Environment::Is32BitOS() || Environment::Is64BitOS()));
    REQUIRE((Environment::Is32BitProcess() || Environment::Is64BitProcess()));
    REQUIRE((Environment::IsDebug() || Environment::IsRelease()));
    REQUIRE(Environment::OSVersion().length() > 0);
    REQUIRE(Environment::Timestamp() > 0);
}
