if(NOT Catch2)

  # Module subdirectory
  add_subdirectory("Catch2")

  # Module folder
  set_target_properties(Catch2 PROPERTIES FOLDER "modules/Catch2")
  set_target_properties(Catch2WithMain PROPERTIES FOLDER "modules/Catch2")

endif()
