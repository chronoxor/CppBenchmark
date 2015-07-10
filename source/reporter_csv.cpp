//
// Created by Ivan Shynkarenka on 10.07.2015.
//

#include "reporter_csv.h"

namespace CppBenchmark {

void ReporterCSV::ReportHeader()
{
    _stream << "name,avg_time,min_time,max_time,total_time,total_iterations,total_items,total_bytes,iterations_per_second,items_per_second,bytes_per_second" << std::endl;
}

void ReporterCSV::ReportPhase(const PhaseCore& phase, const PhaseMetrics& metrics)
{
    _stream
    << '"' << phase.name() << '"'
    << metrics.avg_time()
    << metrics.min_time()
    << metrics.max_time()
    << metrics.total_time()
    << metrics.total_iterations()
    << metrics.total_items()
    << metrics.total_bytes()
    << metrics.iterations_per_second()
    << metrics.items_per_second()
    << metrics.bytes_per_second()
    << std::endl;
}

} // namespace CppBenchmark
