cmake_minimum_required(VERSION 3.2)
project(cppbenchmark)

# Compiler features
include(cmake/SetCompilerFeatures.cmake)
include(cmake/SetCompilerWarningAll.cmake)
include(cmake/SetCrtSecureWarningsOff.cmake)

# Library
include_directories(include)
file(GLOB_RECURSE HEADER_FILES "source/*.h")
file(GLOB_RECURSE SOURCE_FILES "source/*.cpp")
add_library(cppbenchmark ${HEADER_FILES} ${SOURCE_FILES})

# Examples
add_executable(cppbenchmark-example-atomic examples/atomic.cpp)
target_link_libraries(cppbenchmark-example-atomic cppbenchmark)
add_executable(cppbenchmark-example-containers examples/containers.cpp)
target_link_libraries(cppbenchmark-example-containers cppbenchmark)
add_executable(cppbenchmark-example-fwrite examples/fwrite.cpp)
target_link_libraries(cppbenchmark-example-fwrite cppbenchmark)
add_executable(cppbenchmark-example-mpmc examples/mpmc.cpp)
target_link_libraries(cppbenchmark-example-mpmc cppbenchmark)
add_executable(cppbenchmark-example-spsc examples/spsc.cpp)
target_link_libraries(cppbenchmark-example-spsc cppbenchmark)
add_executable(cppbenchmark-example-sort examples/sort.cpp)
target_link_libraries(cppbenchmark-example-sort cppbenchmark)
add_executable(cppbenchmark-example-threads examples/threads.cpp)
target_link_libraries(cppbenchmark-example-threads cppbenchmark)

# Tests
file(GLOB_RECURSE TESTS_HEADER_FILES "tests/*.h")
file(GLOB_RECURSE TESTS_SOURCE_FILES "tests/*.cpp")
add_executable(cppbenchmark-tests ${TESTS_HEADER_FILES} ${TESTS_SOURCE_FILES})
target_link_libraries(cppbenchmark-tests cppbenchmark)

# CTest
enable_testing()
add_test(cppbenchmark-tests cppbenchmark-tests)
