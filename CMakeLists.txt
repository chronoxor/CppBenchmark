cmake_minimum_required(VERSION 3.2)
project(cppbenchmark)

# Compiler features
include("cmake/SetCompilerFeatures.cmake")
include("cmake/SetCompilerWarningAll.cmake")
include("cmake/SetCrtSecureWarningsOff.cmake")

# External packages
find_package(Threads REQUIRED)

# Modules
add_subdirectory("modules")

# Library
include_directories("include")
include_directories("modules/cpp-optparse")
file(GLOB_RECURSE HEADER_FILES "source/*.h")
file(GLOB_RECURSE SOURCE_FILES "source/*.cpp")
add_library(cppbenchmark ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(cppbenchmark cpp-optparse)
list(APPEND INSTALL_TARGETS cppbenchmark)

# Examples
file(GLOB EXAMPLE_FILES RELATIVE "${PROJECT_SOURCE_DIR}/examples" "examples/*.cpp")
foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
  string(REGEX REPLACE "(.*)\\.cpp" "\\1" EXAMPLE_NAME ${EXAMPLE_FILE})
  set(EXAMPLE_TARGET "cppbenchmark-example-${EXAMPLE_NAME}")
  add_executable(${EXAMPLE_TARGET} examples/${EXAMPLE_FILE})
  target_link_libraries(${EXAMPLE_TARGET} cppbenchmark Threads::Threads)
  list(APPEND INSTALL_TARGETS ${EXAMPLE_TARGET})
endforeach()

# Tests
include_directories("modules/catch/single_include")
file(GLOB_RECURSE TESTS_HEADER_FILES "tests/*.h")
file(GLOB_RECURSE TESTS_SOURCE_FILES "tests/*.cpp")
add_executable(cppbenchmark-tests ${CATCH_HEADER} ${TESTS_HEADER_FILES} ${TESTS_SOURCE_FILES})
target_link_libraries(cppbenchmark-tests cppbenchmark Threads::Threads)
list(APPEND INSTALL_TARGETS cppbenchmark-tests)

# CTest
enable_testing()
add_test(cppbenchmark-tests cppbenchmark-tests)

# Install
install(TARGETS ${INSTALL_TARGETS}
  RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/bin"
  LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/bin"
  ARCHIVE DESTINATION "${PROJECT_SOURCE_DIR}/bin")

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN "doxygen")
  add_custom_command(OUTPUT "Doxyfile" COMMAND ${DOXYGEN_EXECUTABLE} "Doxyfile" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/documents")
  add_custom_target(${DOXYGEN} DEPENDS "Doxyfile")
endif()
