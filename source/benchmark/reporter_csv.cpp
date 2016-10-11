/*!
    \file reporter_csv.cpp
    \brief Comma-separated values (CSV) reporter implementation
    \author Ivan Shynkarenka
    \date 10.07.2015
    \copyright MIT License
*/

#include "benchmark/reporter_csv.h"

namespace CppBenchmark {

void ReporterCSV::ReportHeader()
{
    _stream << "name,avg_time,min_time,max_time,total_time,total_iterations,total_items,total_bytes,iterations_per_second,items_per_second,bytes_per_second\n";
}

void ReporterCSV::ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics)
{
    _stream << '"' << phase.name() << '"' << ',';

    if (metrics.latency())
    {
        _stream
        << metrics.min_latency() << ','
        << metrics.max_latency() << ','
        << metrics.mean_latency() << ','
        << metrics.stdv_latency() << ',';
    }

    _stream
    << metrics.avg_time() << ','
    << metrics.min_time() << ','
    << metrics.max_time() << ','
    << metrics.total_time() << ','
    << metrics.total_iterations() << ','
    << metrics.total_items() << ','
    << metrics.total_bytes() << ','
    << metrics.iterations_per_second() << ','
    << metrics.items_per_second() << ','
    << metrics.bytes_per_second() << '\n';
}

} // namespace CppBenchmark
