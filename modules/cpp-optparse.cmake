if(NOT TARGET cpp-optparse)

  # Add module library
  add_library(cpp-optparse cpp-optparse/OptionParser.h cpp-optparse/OptionParser.cpp)

  # Set module folder
  set_target_properties(cpp-optparse PROPERTIES FOLDER modules/cpp-optparse)

endif()
