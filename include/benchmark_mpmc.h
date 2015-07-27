//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#ifndef CPPBENCHMARK_BENCHMARK_MPMC_H
#define CPPBENCHMARK_BENCHMARK_MPMC_H

#include <thread>

#include "benchmark_threads.h"
#include "context_mpmc.h"
#include "fixture_mpmc.h"
#include "settings_mpmc.h"

namespace CppBenchmark {

class BenchmarkMPMC : public BenchmarkThreads, public virtual FixtureMPMC
{
public:
    typedef SettingsMPMC TSettings;		
	
    explicit BenchmarkMPMC(const std::string& name, const TSettings& settings = TSettings()) : BenchmarkThreads(name, settings) {}
    BenchmarkMPMC(const BenchmarkMPMC&) = delete;
    BenchmarkMPMC(BenchmarkMPMC&&) = delete;
    virtual ~BenchmarkMPMC() = default;

    BenchmarkMPMC& operator=(const BenchmarkMPMC&) = delete;
    BenchmarkMPMC& operator=(BenchmarkMPMC&&) = delete;

protected:
    virtual void RunProducer(ContextMPMC& context) = 0;
    virtual void RunConsumer(ContextMPMC& context) = 0;

private:
    void Launch(LauncherHandler* handler) override;

    // Hide base benchmark run thread method
    void RunThread(ContextThread& context) override {}
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_BENCHMARK_MPMC_H
