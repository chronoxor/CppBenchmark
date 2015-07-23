//
// Created by Ivan Shynkarenka on 10.07.2015.
//

#include "reporter_csv.h"

#include <iomanip>

namespace CppBenchmark {

void ReporterCSV::ReportHeader()
{
    _stream << "name,avg_time,min_time,max_time,total_time,total_iterations,total_items,total_bytes,iterations_per_second,items_per_second,bytes_per_second\n";
}

void ReporterCSV::ReportPhase(const PhaseCore& phase, const PhaseMetrics& best, const PhaseMetrics& worst)
{
    _stream
    << '"' << phase.name() << '"' << ','
    << ReportValue(best.avg_time(), worst.avg_time()) << ','
    << ReportValue(best.min_time(), worst.min_time()) << ','
    << ReportValue(best.max_time(), worst.max_time()) << ','
    << ReportValue(best.total_time(), worst.total_time()) << ','
    << ReportValue(best.total_iterations(), worst.total_iterations()) << ','
    << ReportValue(best.total_items(), worst.total_items()) << ','
    << ReportValue(best.total_bytes(), worst.total_bytes()) << ','
    << std::fixed << std::setprecision(3) << ReportValue(best.iterations_per_second(), worst.iterations_per_second()) << ','
    << std::fixed << std::setprecision(3) << ReportValue(best.items_per_second(), worst.items_per_second()) << ','
    << std::fixed << std::setprecision(3) << ReportValue(best.bytes_per_second(), worst.bytes_per_second()) << '\n';
}

} // namespace CppBenchmark
