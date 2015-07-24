//
// Created by Ivan Shynkarenka on 24.07.2015.
//

#ifndef CPPBENCHMARK_SETTINGS_MPMC_H
#define CPPBENCHMARK_SETTINGS_MPMC_H

#include "settings.h"

namespace CppBenchmark {

class SettingsMPMC : public Settings
{
public:
    SettingsMPMC(int iterations = 1) : Settings(iterations) {}
    SettingsMPMC(int iterations, int producers, int consumers) : SettingsMPMC(iterations) { MPMC(producers, consumers); }
    SettingsMPMC(const Settings& settings) : Settings(settings) {}
    SettingsMPMC(const SettingsMPMC&) = default;
    SettingsMPMC(Settings&& settings) : Settings(settings) {}
    SettingsMPMC(SettingsMPMC&&) = default;
    ~SettingsMPMC() = default;

    SettingsMPMC& operator=(const SettingsMPMC&) = default;
    SettingsMPMC& operator=(SettingsMPMC&&) = default;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_MPMC_H
