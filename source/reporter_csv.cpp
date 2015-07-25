//
// Created by Ivan Shynkarenka on 10.07.2015.
//

#include "reporter_csv.h"

namespace CppBenchmark {

void ReporterCSV::ReportHeader()
{
    _stream << "name,total_time,total_iterations,total_items,total_bytes,time_per_iteration,iterations_per_second,items_per_second,bytes_per_second\n";
}

void ReporterCSV::ReportPhase(const PhaseCore& phase, const PhaseMetrics& best, const PhaseMetrics& worst)
{
    _stream
    << '"' << phase.name() << '"' << ','
    << ReportValue(best.total_time(), worst.total_time()) << ','
    << ReportValue(best.total_iterations(), worst.total_iterations()) << ','
    << ReportValue(best.total_items(), worst.total_items()) << ','
    << ReportValue(best.total_bytes(), worst.total_bytes()) << ','
    << ReportValue(best.time_per_iteration(), worst.time_per_iteration()) << ','
    << ReportValue(best.iterations_per_second(), worst.iterations_per_second()) << ','
    << ReportValue(best.items_per_second(), worst.items_per_second()) << ','
    << ReportValue(best.bytes_per_second(), worst.bytes_per_second()) << '\n';
}

} // namespace CppBenchmark
