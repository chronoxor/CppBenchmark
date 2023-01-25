if(NOT TARGET HdrHistogram)

  # Module library
  file(GLOB SOURCE_FILES "HdrHistogram/src/*.c")
  list(FILTER SOURCE_FILES EXCLUDE REGEX ".*/hdr_histogram_log_no_op.c")
  add_library(HdrHistogram ${SOURCE_FILES})
  if(MSVC)
    # C4057: 'operator' : 'identifier1' indirection to slightly different base types from 'identifier2'
    # C4146: unary minus operator applied to unsigned type, result still unsigned
    # C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
    set_target_properties(HdrHistogram PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} /wd4057 /wd4146 /wd4244")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set_target_properties(HdrHistogram PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} -Wno-char-subscripts -Wno-format -Wno-implicit-function-declaration -Wno-incompatible-pointer-types -Wno-restrict")
  else()
    set_target_properties(HdrHistogram PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS}")
  endif()
  target_include_directories(HdrHistogram PUBLIC "HdrHistogram/include")
  target_link_libraries(HdrHistogram zlib)

  # Module folder
  set_target_properties(HdrHistogram PROPERTIES FOLDER "modules/HdrHistogram")

endif()
