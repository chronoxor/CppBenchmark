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
    //! Initialize settings with the given count of operations
    /*!
        \param operations - Count of operations (default is 1)
    */
    SettingsPC(int64_t operations = 1) : Settings(operations) {}
    //! Initialize settings with the given count of operations and count of producers/consumers
    /*!
        \param operations - Count of operations
        \param producers - Count of producers
        \param consumers - Count of consumers
    */
    SettingsPC(int64_t operations, int producers, int consumers) : SettingsPC(operations) { PC(producers, consumers); }
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
