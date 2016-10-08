cmake_minimum_required(VERSION 2.8)

# Global properties
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Project name
project(cppbenchmark)

# Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN "doxygen")
  if(NOT TARGET ${DOXYGEN})
    add_custom_command(OUTPUT "Doxyfile" COMMAND ${DOXYGEN_EXECUTABLE} "Doxyfile" WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/documents")
    add_custom_target(${DOXYGEN} DEPENDS "Doxyfile")
    set_target_properties(${DOXYGEN} PROPERTIES FOLDER doxygen)
  endif()
endif()

# CMake module path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Compiler features
include(SetCompilerFeatures)
include(SetCompilerWarningAll)
include(SetCrtSecureWarningsOff)
include(SetPlatformFeatures)

# External packages
find_package(Threads REQUIRED)

# System link libraries
list(APPEND SYSTEM Threads::Threads)

# Modules
add_subdirectory("modules")

# Includes
include_directories("include")
include_directories("modules/catch/single_include")
include_directories("modules/cpp-optparse")
include_directories("modules/HdrHistogram/src")
include_directories("modules/zlib")
include_directories("build/modules/zlib")

# Library
file(GLOB_RECURSE SOURCE_FILES "source/*.cpp")
add_library(cppbenchmark ${SOURCE_FILES})
target_link_libraries(cppbenchmark cpp-optparse histogram)
set_target_properties(cppbenchmark PROPERTIES FOLDER libraries)
list(APPEND INSTALL_TARGETS cppbenchmark)

# Full module components: examples, tests and install
if(NOT CPPBENCHMARK_MODULE)

  # Examples
  file(GLOB EXAMPLE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/examples" "examples/*.cpp")
  foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
    string(REGEX REPLACE "(.*)\\.cpp" "\\1" EXAMPLE_NAME ${EXAMPLE_FILE})
    set(EXAMPLE_TARGET "cppbenchmark-example-${EXAMPLE_NAME}")
    add_executable(${EXAMPLE_TARGET} examples/${EXAMPLE_FILE})
    target_link_libraries(${EXAMPLE_TARGET} cppbenchmark ${SYSTEM})
    set_target_properties(${EXAMPLE_TARGET} PROPERTIES FOLDER examples)
    list(APPEND INSTALL_TARGETS ${EXAMPLE_TARGET})
    list(APPEND INSTALL_TARGETS_PDB ${EXAMPLE_TARGET})
  endforeach()

  # Tests
  file(GLOB TESTS_SOURCE_FILES "tests/*.cpp")
  add_executable(cppbenchmark-tests ${CATCH_HEADER} ${TESTS_SOURCE_FILES})
  target_link_libraries(cppbenchmark-tests cppbenchmark ${SYSTEM})
  set_target_properties(cppbenchmark-tests PROPERTIES FOLDER tests)
  list(APPEND INSTALL_TARGETS cppbenchmark-tests)
  list(APPEND INSTALL_TARGETS_PDB cppbenchmark-tests)

  # CTest
  enable_testing()
  add_test(cppbenchmark-tests cppbenchmark-tests)

  # Install
  install(TARGETS ${INSTALL_TARGETS}
    RUNTIME DESTINATION "${PROJECT_SOURCE_DIR}/bin"
    LIBRARY DESTINATION "${PROJECT_SOURCE_DIR}/bin"
    ARCHIVE DESTINATION "${PROJECT_SOURCE_DIR}/bin")

  # Install *.pdb files
  if(MSVC)
    foreach(INSTALL_TARGET_PDB ${INSTALL_TARGETS_PDB})
      install(FILES $<TARGET_PDB_FILE:${INSTALL_TARGET_PDB}> DESTINATION "${PROJECT_SOURCE_DIR}/bin")
    endforeach()
  endif()

endif()
