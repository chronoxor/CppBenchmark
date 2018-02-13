//
// Created by Ivan Shynkarenka on 08.07.2015
//

#include "test.h"

#include "benchmark/system.h"

using namespace CppBenchmark;

TEST_CASE("System information", "[CppBenchmark][System]")
{
    REQUIRE(System::CpuArchitecture().size() >= 0);
    REQUIRE(System::CpuLogicalCores() >= 0);
    REQUIRE(System::CpuPhysicalCores() >= 0);

    std::pair<int, int> total_cores = System::CpuTotalCores();
    REQUIRE(((total_cores.first >= 0) && (total_cores.second >= 0)));

    REQUIRE(System::CpuClockSpeed() >= 0);

    REQUIRE(System::RamTotal() >= 0);
    REQUIRE(System::RamFree() >= 0);
}
