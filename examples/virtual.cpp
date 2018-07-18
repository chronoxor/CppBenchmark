//
// Created by Ivan Shynkarenka on 28.03.2017
//

#include "benchmark/cppbenchmark.h"

#include <array>
#include <memory>

const uint64_t operations = 1000000000;

class TesterBase
{
public:
    TesterBase() = default;
    virtual ~TesterBase() = default;

    inline int TestInline(int i) { return i + 1; }

#if (defined(__clang__) || defined(__GNUC__))
    int TestDirect(int i) __attribute__((noinline)) { return i + 2; }
#elif defined(_MSC_VER)
    __declspec(noinline) int TestDirect(int i) { return i + 2; }
#endif

    virtual int TestVirtual(int i) { return i + 3; }
};

class TesterDerived : public TesterBase
{
public:
    int TestVirtual(int i) override { return i + 33; }
};

class VirtualFixture
{
protected:
    std::array<TesterBase*, 1000> testers;

    VirtualFixture()
    {
        for (size_t i = 0; i < testers.size(); ++i)
            testers[i] = (i % 2) ? new TesterBase() : new TesterDerived();
    }

    ~VirtualFixture()
    {
        for (size_t i = 0; i < testers.size(); ++i)
            delete testers[i];
    }
};

BENCHMARK_FIXTURE(VirtualFixture, "Inline call", 1)
{
    uint64_t crc = 0;
    for (uint64_t i = 0; i < operations; ++i)
        crc += testers[i % testers.size()]->TestInline((int)i);
    context.metrics().AddOperations(operations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(VirtualFixture, "Direct call", 1)
{
    uint64_t crc = 0;
    for (uint64_t i = 0; i < operations; ++i)
        crc += testers[i % testers.size()]->TestDirect((int)i);
    context.metrics().AddOperations(operations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_FIXTURE(VirtualFixture, "Virtual call", 1)
{
    uint64_t crc = 0;
    for (uint64_t i = 0; i < operations; ++i)
        crc += testers[i % testers.size()]->TestVirtual((int)i);
    context.metrics().AddOperations(operations - 1);
    context.metrics().SetCustom("CRC", crc);
}

BENCHMARK_MAIN()
