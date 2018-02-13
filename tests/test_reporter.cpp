//
// Created by Ivan Shynkarenka on 14.07.2015
//

#include "test.h"

#include "benchmark/reporter_console.h"

using namespace CppBenchmark;

TEST_CASE("Clock speed string generator", "[CppBenchmark][Reporter][Console]")
{
    REQUIRE(ReporterConsole::GenerateClockSpeed(0ll) == "0 Hz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(512ll) == "512 Hz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(999ll) == "999 Hz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(1000ll) == "1.000 kHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(1001ll) == "1.001 kHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(1500ll) == "1.500 kHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(1543ll) == "1.543 kHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(999999ll) == "999.999 kHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(1000000ll) == "1.000 MHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(1001234ll) == "1.001 MHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(500001234ll) == "500.001 MHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(999999999ll) == "999.999 MHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(1000000000ll) == "1.000 GHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(500000000000ll) == "500.000 GHz");
    REQUIRE(ReporterConsole::GenerateClockSpeed(500123456789ll) == "500.123 GHz");
}

TEST_CASE("Data size string generator", "[CppBenchmark][Reporter][Console]")
{
    REQUIRE(ReporterConsole::GenerateDataSize(0ll) == "0 bytes");
    REQUIRE(ReporterConsole::GenerateDataSize(512ll) == "512 bytes");
    REQUIRE(ReporterConsole::GenerateDataSize(1023ll) == "1023 bytes");
    REQUIRE(ReporterConsole::GenerateDataSize(1024ll) == "1.000 KiB");
    REQUIRE(ReporterConsole::GenerateDataSize(1025ll) == "1.001 KiB");
    REQUIRE(ReporterConsole::GenerateDataSize(524288ll) == "512.000 KiB");
    REQUIRE(ReporterConsole::GenerateDataSize(525299ll) == "512.1011 KiB");
    REQUIRE(ReporterConsole::GenerateDataSize(1048575ll) == "1023.1023 KiB");
    REQUIRE(ReporterConsole::GenerateDataSize(1048576ll) == "1.000 MiB");
    REQUIRE(ReporterConsole::GenerateDataSize(1049699ll) == "1.001 MiB");
    REQUIRE(ReporterConsole::GenerateDataSize(536870912ll) == "512.000 MiB");
    REQUIRE(ReporterConsole::GenerateDataSize(536871936ll) == "512.001 MiB");
    REQUIRE(ReporterConsole::GenerateDataSize(1073741823ll) == "1023.1023 MiB");
    REQUIRE(ReporterConsole::GenerateDataSize(1073741824ll) == "1.000 GiB");
    REQUIRE(ReporterConsole::GenerateDataSize(1074790400ll) == "1.001 GiB");
    REQUIRE(ReporterConsole::GenerateDataSize(549755813888ll) == "512.000 GiB");
    REQUIRE(ReporterConsole::GenerateDataSize(549756862464ll) == "512.001 GiB");
    REQUIRE(ReporterConsole::GenerateDataSize(1099511627775ll) == "1023.1023 GiB");
    REQUIRE(ReporterConsole::GenerateDataSize(1099511627776ll) == "1.000 TiB");
    REQUIRE(ReporterConsole::GenerateDataSize(562949953421312ll) == "512.000 TiB");
    REQUIRE(ReporterConsole::GenerateDataSize(562951027163136ll) == "512.001 TiB");
    REQUIRE(ReporterConsole::GenerateDataSize(562951027999999ll) == "512.001 TiB");
}

TEST_CASE("Time period string generator", "[CppBenchmark][Reporter][Console]")
{
    REQUIRE(ReporterConsole::GenerateTimePeriod(0ll) == "0 ns");
    REQUIRE(ReporterConsole::GenerateTimePeriod(500ll) == "500 ns");
    REQUIRE(ReporterConsole::GenerateTimePeriod(999ll) == "999 ns");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1000ll) == "1.000 mcs");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1001ll) == "1.001 mcs");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1500ll) == "1.500 mcs");
    REQUIRE(ReporterConsole::GenerateTimePeriod(500000ll) == "500.000 mcs");
    REQUIRE(ReporterConsole::GenerateTimePeriod(500001ll) == "500.001 mcs");
    REQUIRE(ReporterConsole::GenerateTimePeriod(999999ll) == "999.999 mcs");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1000000ll) == "1.000 ms");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1000001ll) == "1.000 ms");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1001234ll) == "1.001 ms");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1500000ll) == "1.500 ms");
    REQUIRE(ReporterConsole::GenerateTimePeriod(500000000ll) == "500.000 ms");
    REQUIRE(ReporterConsole::GenerateTimePeriod(500001234ll) == "500.001 ms");
    REQUIRE(ReporterConsole::GenerateTimePeriod(999999999ll) == "999.999 ms");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1000000000ll) == "1.000 s");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1000000001ll) == "1.000 s");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1001234567ll) == "1.001 s");
    REQUIRE(ReporterConsole::GenerateTimePeriod(1500000000ll) == "1.500 s");
    REQUIRE(ReporterConsole::GenerateTimePeriod(59000000000ll) == "59.000 s");
    REQUIRE(ReporterConsole::GenerateTimePeriod(59999123456ll) == "59.999 s");
    REQUIRE(ReporterConsole::GenerateTimePeriod(59999999999ll) == "59.999 s");
    REQUIRE(ReporterConsole::GenerateTimePeriod(60000000000ll) == "1:00.000 m");
    REQUIRE(ReporterConsole::GenerateTimePeriod(60000000001ll) == "1:00.000 m");
    REQUIRE(ReporterConsole::GenerateTimePeriod(60001234567ll) == "1:00.001 m");
    REQUIRE(ReporterConsole::GenerateTimePeriod(90000000000ll) == "1:30.000 m");
    REQUIRE(ReporterConsole::GenerateTimePeriod(90123456789ll) == "1:30.123 m");
    REQUIRE(ReporterConsole::GenerateTimePeriod(3600000000000ll) == "1:00:00.000 h");
    REQUIRE(ReporterConsole::GenerateTimePeriod(5430123456789ll) == "1:30:30.123 h");
    REQUIRE(ReporterConsole::GenerateTimePeriod(108000000000000ll) == "30:00:00.000 h");
}
