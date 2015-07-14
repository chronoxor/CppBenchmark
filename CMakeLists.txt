cmake_minimum_required(VERSION 3.2)
project(cppbenchmark)

# Compiler features
include(cmake/SetCompilerFeatures.cmake)
include(cmake/SetCompilerWarningAll.cmake)

# Library
include_directories(include)
set(SOURCE_FILES
    source/benchmark.cpp
    source/context.cpp
    source/environment.cpp
    source/launcher.cpp
    source/phase.cpp
    source/phase_core.cpp
    source/phase_metrics.cpp
    source/phase_scope.cpp
    source/reporter.cpp
    source/reporter_console.cpp
    source/reporter_csv.cpp
    source/reporter_json.cpp
    source/settings.cpp
    source/system.cpp)
add_library(cppbenchmark ${SOURCE_FILES})

# Tests
set(TESTS_SOURCE_FILES
    tests/catch.cpp
    tests/test_environment.cpp
    tests/test_launcher.cpp
    tests/test_reporter.cpp
    tests/test_system.cpp)
add_executable(cppbenchmark-tests ${TESTS_SOURCE_FILES})
target_link_libraries(cppbenchmark-tests cppbenchmark)

# CTest
enable_testing()
add_test(cppbenchmark-tests cppbenchmark-tests)
