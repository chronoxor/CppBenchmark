if(NOT TARGET HdrHistogram)

  # Module library
  file(GLOB SOURCE_FILES "HdrHistogram/src/*.c")
  if(MSVC)
    # C4057: 'operator' : 'identifier1' indirection to slightly different base types from 'identifier2'
    # C4146: unary minus operator applied to unsigned type, result still unsigned
    # C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} /wd4057 /wd4146 /wd4244")
  else()
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} -Wno-format -Wno-implicit-function-declaration -Wno-incompatible-pointer-types")
  endif()
  add_library(HdrHistogram ${SOURCE_FILES})
  target_include_directories(HdrHistogram PUBLIC "HdrHistogram/src")
  target_link_libraries(HdrHistogram zlib)

  # Module folder
  set_target_properties(HdrHistogram PROPERTIES FOLDER modules/HdrHistogram)

endif()
