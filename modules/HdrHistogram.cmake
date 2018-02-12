if(NOT TARGET HdrHistogram)

  # Module library
  file(GLOB SOURCE_FILES "HdrHistogram/src/*.c")
  if(NOT MSVC)
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} -Wno-implicit-function-declaration -Wno-incompatible-pointer-types")
  else()
    # C4146: unary minus operator applied to unsigned type, result still unsigned
    # C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} /wd4146 /wd4244")
  endif()
  add_library(HdrHistogram ${SOURCE_FILES})
  target_include_directories(HdrHistogram PRIVATE "zlib" PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/zlib")
  target_link_libraries(HdrHistogram zlib)

  # Module folder
  set_target_properties(HdrHistogram PROPERTIES FOLDER modules/HdrHistogram)

endif()
