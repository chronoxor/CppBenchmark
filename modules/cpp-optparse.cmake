if(NOT TARGET cpp-optparse)

  # Module library
  file(GLOB SOURCE_FILES "cpp-optparse/OptionParser.cpp")
  add_library(cpp-optparse ${SOURCE_FILES})
  target_include_directories(cpp-optparse PUBLIC "cpp-optparse")

  # Module folder
  set_target_properties(cpp-optparse PROPERTIES FOLDER "modules/cpp-optparse")

endif()
