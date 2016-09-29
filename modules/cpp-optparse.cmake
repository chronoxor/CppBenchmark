if(NOT TARGET cpp-optparse)
  add_library(cpp-optparse cpp-optparse/OptionParser.h cpp-optparse/OptionParser.cpp)
  set_target_properties(cpp-optparse PROPERTIES FOLDER modules/cpp-optparse)
endif()
