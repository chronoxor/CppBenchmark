cmake_minimum_required(VERSION 3.2)
project(cppbenchmark)

# Compiler features
include(cmake/SetCompilerFeatures.cmake)
include(cmake/SetCompilerWarningAll.cmake)
include(cmake/SetCrtSecureWarningsOff.cmake)

# Library
include_directories(include)
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp")
add_library(cppbenchmark ${HEADER_FILES} ${SOURCE_FILES})

# Examples
file(GLOB EXAMPLE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/examples" "${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp")
foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
  string(REGEX REPLACE "(.*)\\.cpp" "\\1" EXAMPLE_NAME ${EXAMPLE_FILE})
  add_executable(cppbenchmark-example-${EXAMPLE_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/examples/${EXAMPLE_FILE})
  target_link_libraries(cppbenchmark-example-${EXAMPLE_NAME} cppbenchmark)
endforeach()

# Tests
file(GLOB_RECURSE TESTS_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.h")
file(GLOB_RECURSE TESTS_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
add_executable(cppbenchmark-tests ${TESTS_HEADER_FILES} ${TESTS_SOURCE_FILES})
target_link_libraries(cppbenchmark-tests cppbenchmark)

# CTest
enable_testing()
add_test(cppbenchmark-tests cppbenchmark-tests)
