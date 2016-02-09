/*!
    \file settings_pc.h
    \brief Producers/Consumers benchmark settings definition
    \author Ivan Shynkarenka
    \date 24.07.2015
    \copyright MIT License
*/

#ifndef CPPBENCHMARK_SETTINGS_PC_H
#define CPPBENCHMARK_SETTINGS_PC_H

#include "settings_threads.h"

namespace CppBenchmark {

//! Producers/Consumers benchmark settings
/*!
    Constructs benchmark settings by providing count of iterations and count of producers/consumers.
*/
class SettingsPC : public SettingsThreads
{
public:
    //! Initialize settings with the given count of iterations
    /*!
        \param iterations - Count of iterations (default is 1)
    */
    SettingsPC(int iterations = 1) : SettingsThreads(iterations) {}
    //! Initialize settings with the given count of iterations and count of producers/consumers
    /*!
        \param iterations - Count of iterations
        \param producers - Count of producers
        \param consumers - Count of consumers
    */
    SettingsPC(int iterations, int producers, int consumers) : SettingsPC(iterations) { PC(producers, consumers); }
    SettingsPC(const Settings& settings) : SettingsThreads(settings) {}
    SettingsPC(const SettingsPC&) = default;
    SettingsPC(Settings&& settings) : SettingsThreads(settings) {}
    SettingsPC(SettingsPC&&) = default;
    ~SettingsPC() = default;

    SettingsPC& operator=(const SettingsPC&) = default;
    SettingsPC& operator=(SettingsPC&&) = default;
};

} // namespace CppBenchmark

#endif // CPPBENCHMARK_SETTINGS_PC_H
