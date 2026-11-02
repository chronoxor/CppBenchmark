if(NOT TARGET zlibstatic)

  # Module flag
  set(ZLIB_BUILD_TESTING OFF)
  set(ZLIB_BUILD_SHARED OFF)
  set(ZLIB_BUILD_STATIC ON)
  set(ZLIB_INSTALL OFF)

  # Module subdirectory
  add_subdirectory("zlib")

  # Module folder
  set_target_properties(zlibstatic PROPERTIES FOLDER "modules/zlib")

endif()
