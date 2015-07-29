cmake_minimum_required(VERSION 3.2)
project(cppbenchmark)

# Compiler features
include(cmake/SetCompilerFeatures.cmake)
include(cmake/SetCompilerWarningAll.cmake)
include(cmake/SetCrtSecureWarningsOff.cmake)

# Library
include_directories(include)
file(GLOB_RECURSE HEADER_FILES "${PROJECT_SOURCE_DIR}/source/*.h")
file(GLOB_RECURSE SOURCE_FILES "${PROJECT_SOURCE_DIR}/source/*.cpp")
add_library(cppbenchmark ${HEADER_FILES} ${SOURCE_FILES})
list(APPEND INSTALL_TARGETS cppbenchmark)

# Examples
file(GLOB EXAMPLE_FILES RELATIVE "${PROJECT_SOURCE_DIR}/examples" "${PROJECT_SOURCE_DIR}/examples/*.cpp")
foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
  string(REGEX REPLACE "(.*)\\.cpp" "\\1" EXAMPLE_NAME ${EXAMPLE_FILE})
  set(EXAMPLE_TARGET "cppbenchmark-example-${EXAMPLE_NAME}")
  add_executable(${EXAMPLE_TARGET} ${PROJECT_SOURCE_DIR}/examples/${EXAMPLE_FILE})
  target_link_libraries(${EXAMPLE_TARGET} cppbenchmark)
  list(APPEND INSTALL_TARGETS ${EXAMPLE_TARGET})
endforeach()

# Tests
file(GLOB_RECURSE TESTS_HEADER_FILES "${PROJECT_SOURCE_DIR}/tests/*.h")
file(GLOB_RECURSE TESTS_SOURCE_FILES "${PROJECT_SOURCE_DIR}/tests/*.cpp")
add_executable(cppbenchmark-tests ${TESTS_HEADER_FILES} ${TESTS_SOURCE_FILES})
target_link_libraries(cppbenchmark-tests cppbenchmark)
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
  set(DOXYGEN "Doxygen")
  project(${DOXYGEN})
  add_custom_command(OUTPUT "Doxyfile" COMMAND ${DOXYGEN_EXECUTABLE} "${PROJECT_SOURCE_DIR}/documents/Doxyfile" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/documents")
  add_custom_target(${DOXYGEN} DEPENDS "Doxyfile")
endif()
