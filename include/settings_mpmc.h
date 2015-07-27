//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#ifndef CPPBENCHMARK_SETTINGS_MPMC_H
#define CPPBENCHMARK_SETTINGS_MPMC_H

#include "settings_threads.h"

namespace CppBenchmark {

class SettingsMPMC : public SettingsThreads
{
public:
    SettingsMPMC(int iterations = 1) : SettingsThreads(iterations) {}
    SettingsMPMC(int iterations, int producers, int consumers) : SettingsMPMC(iterations) { MPMC(producers, consumers); }
    SettingsMPMC(const Settings& settings) : SettingsThreads(settings) {}
    SettingsMPMC(const SettingsMPMC&) = default;
    SettingsMPMC(Settings&& settings) : SettingsThreads(settings) {}
    SettingsMPMC(SettingsMPMC&&) = default;
    ~SettingsMPMC() = default;

    SettingsMPMC& operator=(const SettingsMPMC&) = default;
    SettingsMPMC& operator=(SettingsMPMC&&) = default;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_MPMC_H
