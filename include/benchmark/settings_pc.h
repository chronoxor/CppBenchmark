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
    Constructs benchmark settings by providing count of iterations and count of producers/consumers.
*/
class SettingsPC : public Settings
{
public:
    //! Initialize settings with the given count of iterations
    /*!
        \param iterations - Count of iterations (default is 1)
    */
    SettingsPC(int64_t iterations = 1) : Settings(iterations) {}
    //! Initialize settings with the given count of iterations and count of producers/consumers
    /*!
        \param iterations - Count of iterations
        \param producers - Count of producers
        \param consumers - Count of consumers
    */
    SettingsPC(int64_t iterations, int producers, int consumers) : SettingsPC(iterations) { PC(producers, consumers); }
    SettingsPC(const Settings& settings) : Settings(settings) {}
    SettingsPC(const SettingsPC&) = default;
    SettingsPC(Settings&& settings) : Settings(settings) {}
    SettingsPC(SettingsPC&&) = default;
    ~SettingsPC() = default;

    SettingsPC& operator=(const SettingsPC&) = default;
    SettingsPC& operator=(SettingsPC&&) = default;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_PC_H
