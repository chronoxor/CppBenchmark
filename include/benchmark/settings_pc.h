/*!
    \file settings_pc.h
    \brief Producers/Consumers benchmark settings definition
    \author Ivan Shynkarenka
    \date 24.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_SETTINGS_PC_H
#define CPPBENCHMARK_SETTINGS_PC_H

#include "benchmark/settings.h"

namespace CppBenchmark {

//! Producers/Consumers benchmark settings
/*!
    Constructs benchmark settings by providing count of operations and count of producers/consumers.
*/
class SettingsPC : public Settings
{
public:
    //! Initialize settings with the default benchmark duration (5 seconds)
    SettingsPC() : Settings() {}
    //! Initialize settings with the given count of producers/consumers
    /*!
        \param producers - Count of producers
        \param consumers - Count of consumers
    */
    SettingsPC(int producers, int consumers) : Settings() { PC(producers, consumers); }
    //! Initialize settings with the given count of producers/consumers and the given count of operations
    /*!
        \param producers - Count of producers
        \param consumers - Count of consumers
        \param operations - Count of operations
    */
    SettingsPC(int producers, int consumers, int64_t operations) : Settings(operations) { PC(producers, consumers); }
    SettingsPC(const Settings& settings) : Settings(settings) {}
    SettingsPC(const SettingsPC&) = default;
    SettingsPC(Settings&& settings) noexcept : Settings(std::move(settings)) {}
    SettingsPC(SettingsPC&&) noexcept = default;
    ~SettingsPC() = default;

    SettingsPC& operator=(const SettingsPC&) = default;
    SettingsPC& operator=(SettingsPC&&) noexcept = default;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_PC_H
