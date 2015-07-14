//
// Created by Ivan Shynkarenka on 14.07.2015.
//

#include "catch.hpp"

#include "reporter_console.h"

using namespace CppBenchmark;

TEST_CASE("Size string generator", "[CppBenchmark][Reporter][Console]")
{
    REQUIRE(ReporterConsole::GenerateSize(0ll) == "0 bytes");
    REQUIRE(ReporterConsole::GenerateSize(512ll) == "512 bytes");
    REQUIRE(ReporterConsole::GenerateSize(1023ll) == "1023 bytes");
    REQUIRE(ReporterConsole::GenerateSize(1024ll) == "1.000 KiB");
    REQUIRE(ReporterConsole::GenerateSize(1025ll) == "1.001 KiB");
    REQUIRE(ReporterConsole::GenerateSize(524288ll) == "512.000 KiB");
    REQUIRE(ReporterConsole::GenerateSize(525299ll) == "512.1011 KiB");
    REQUIRE(ReporterConsole::GenerateSize(1048575ll) == "1023.1023 KiB");
    REQUIRE(ReporterConsole::GenerateSize(1048576ll) == "1.000 MiB");
    REQUIRE(ReporterConsole::GenerateSize(1049699ll) == "1.001 MiB");
    REQUIRE(ReporterConsole::GenerateSize(536870912ll) == "512.000 MiB");
    REQUIRE(ReporterConsole::GenerateSize(536871936ll) == "512.001 MiB");
    REQUIRE(ReporterConsole::GenerateSize(1073741823ll) == "1023.1023 MiB");
    REQUIRE(ReporterConsole::GenerateSize(1073741824ll) == "1.000 GiB");
    REQUIRE(ReporterConsole::GenerateSize(1074790400ll) == "1.001 GiB");
    REQUIRE(ReporterConsole::GenerateSize(549755813888ll) == "512.000 GiB");
    REQUIRE(ReporterConsole::GenerateSize(549756862464ll) == "512.001 GiB");
    REQUIRE(ReporterConsole::GenerateSize(1099511627775ll) == "1023.1023 GiB");
    REQUIRE(ReporterConsole::GenerateSize(1099511627776ll) == "1.000 TiB");
    REQUIRE(ReporterConsole::GenerateSize(562949953421312ll) == "512.000 TiB");
    REQUIRE(ReporterConsole::GenerateSize(562951027163136ll) == "512.001 TiB");
    REQUIRE(ReporterConsole::GenerateSize(562951027999999ll) == "512.001 TiB");
}

TEST_CASE("Time string generator", "[CppBenchmark][Reporter][Console]")
{
    REQUIRE(ReporterConsole::GenerateTime(0ll) == "0 ns");
    REQUIRE(ReporterConsole::GenerateTime(500ll) == "500 ns");
    REQUIRE(ReporterConsole::GenerateTime(999ll) == "999 ns");
    REQUIRE(ReporterConsole::GenerateTime(1000ll) == "1.000 mcs");
    REQUIRE(ReporterConsole::GenerateTime(1001ll) == "1.001 mcs");
    REQUIRE(ReporterConsole::GenerateTime(1500ll) == "1.500 mcs");
    REQUIRE(ReporterConsole::GenerateTime(500000ll) == "500.000 mcs");
    REQUIRE(ReporterConsole::GenerateTime(500001ll) == "500.001 mcs");
    REQUIRE(ReporterConsole::GenerateTime(999999ll) == "999.999 mcs");
    REQUIRE(ReporterConsole::GenerateTime(1000000ll) == "1.000 ms");
    REQUIRE(ReporterConsole::GenerateTime(1000001ll) == "1.000 ms");
    REQUIRE(ReporterConsole::GenerateTime(1001234ll) == "1.001 ms");
    REQUIRE(ReporterConsole::GenerateTime(1500000ll) == "1.500 ms");
    REQUIRE(ReporterConsole::GenerateTime(500000000ll) == "500.000 ms");
    REQUIRE(ReporterConsole::GenerateTime(500001234ll) == "500.001 ms");
    REQUIRE(ReporterConsole::GenerateTime(999999999ll) == "999.999 ms");
    REQUIRE(ReporterConsole::GenerateTime(1000000000ll) == "1.000 s");
    REQUIRE(ReporterConsole::GenerateTime(1000000001ll) == "1.000 s");
    REQUIRE(ReporterConsole::GenerateTime(1001234567ll) == "1.001 s");
    REQUIRE(ReporterConsole::GenerateTime(1500000000ll) == "1.500 s");
    REQUIRE(ReporterConsole::GenerateTime(59000000000ll) == "59.000 s");
    REQUIRE(ReporterConsole::GenerateTime(59999123456ll) == "59.999 s");
    REQUIRE(ReporterConsole::GenerateTime(59999999999ll) == "59.999 s");
    REQUIRE(ReporterConsole::GenerateTime(60000000000ll) == "1:00.000 m");
    REQUIRE(ReporterConsole::GenerateTime(60000000001ll) == "1:00.000 m");
    REQUIRE(ReporterConsole::GenerateTime(60001234567ll) == "1:00.001 m");
    REQUIRE(ReporterConsole::GenerateTime(90000000000ll) == "1:30.000 m");
    REQUIRE(ReporterConsole::GenerateTime(90123456789ll) == "1:30.123 m");
    REQUIRE(ReporterConsole::GenerateTime(3600000000000ll) == "1:00:00.000 h");
    REQUIRE(ReporterConsole::GenerateTime(5430123456789ll) == "1:30:30.123 h");
    REQUIRE(ReporterConsole::GenerateTime(108000000000000ll) == "30:00:00.000 h");
}
