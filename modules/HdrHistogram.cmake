if(NOT TARGET histogram)

  # Sources
  file(GLOB_RECURSE SOURCE_FILES "HdrHistogram/src/*.c")

  # Temporary disable some warnings
  # C4146: unary minus operator applied to unsigned type, result still unsigned
  # C4200: nonstandard extension used : zero-sized array in struct/union
  # C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
  if(CMAKE_MAKE_PROGRAM MATCHES "(MSBuild|devenv|msdev|nmake)")
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "/wd4146 /wd4200 /wd4244")
  elseif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUC)
    set(CMAKE_OLD_C_FLAGS ${CMAKE_C_FLAGS})
    set(CMAKE_OLD_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS_INIT} -Wall -Werror" FORCE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_INIT} -Wall -Werror" FORCE)
  endif()

  # Includes
  include_directories("zlib")
  include_directories("${CMAKE_CURRENT_BINARY_DIR}/zlib")

  # Add module library
  add_library(histogram ${SOURCE_FILES})
  target_link_libraries(histogram zlibstatic)

  # Set module folder
  set_target_properties(histogram PROPERTIES FOLDER modules/HdrHistogram)

  # Restore default warnings
  if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUC)
    set(CMAKE_C_FLAGS ${CMAKE_OLD_C_FLAGS} FORCE)
    set(CMAKE_CXX_FLAGS ${CMAKE_OLD_CXX_FLAGS} FORCE)
  endif()

endif()
