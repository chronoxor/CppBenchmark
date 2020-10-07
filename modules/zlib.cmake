if(NOT TARGET zlib)

  # Module configuration
  include(CheckTypeSize)
  include(CheckFunctionExists)
  include(CheckIncludeFile)
  include(CheckCSourceCompiles)
  check_include_file(sys/types.h HAVE_SYS_TYPES_H)
  check_include_file(stdint.h HAVE_STDINT_H)
  check_include_file(stddef.h HAVE_STDDEF_H)
  set(CMAKE_REQUIRED_DEFINITIONS -D_LARGEFILE64_SOURCE=1)
  if(HAVE_SYS_TYPES_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_SYS_TYPES_H)
  endif()
  if(HAVE_STDINT_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDINT_H)
  endif()
  if(HAVE_STDDEF_H)
    list(APPEND CMAKE_REQUIRED_DEFINITIONS -DHAVE_STDDEF_H)
  endif()
  check_type_size(off64_t OFF64_T)
  if(HAVE_OFF64_T)
    add_definitions(-D_LARGEFILE64_SOURCE=1)
  endif()
  set(CMAKE_REQUIRED_DEFINITIONS)
  check_function_exists(fseeko HAVE_FSEEKO)
  if(NOT HAVE_FSEEKO)
    add_definitions(-DNO_FSEEKO)
  endif()
  check_include_file(unistd.h Z_HAVE_UNISTD_H)
  if(Z_HAVE_UNISTD_H)
    add_definitions(-DZ_HAVE_UNISTD_H)
  endif()
  
  # Module definitions
  if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
  endif()

  # Module library
  file(GLOB SOURCE_FILES "zlib/*.c")
  if(MSVC)
    list(APPEND ${SOURCE_FILES} "zlib/contrib/masmx64/inffas8664.c")
    # C4127: conditional expression is constant
    # C4131: 'function' : uses old-style declarator
    # C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
    # C4245: 'conversion' : conversion from 'type1' to 'type2', signed/unsigned mismatch
    # C4267: 'var' : conversion from 'size_t' to 'type', possible loss of data
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} /wd4127 /wd4131 /wd4244 /wd4245 /wd4267")
  else()
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${PEDANTIC_COMPILE_FLAGS} -Wno-implicit-function-declaration -Wno-shift-negative-value")
  endif()
  add_library(zlib ${SOURCE_FILES})
  target_include_directories(zlib PUBLIC "zlib")
  target_link_libraries(zlib)

  # Module folder
  set_target_properties(zlib PROPERTIES FOLDER "modules/zlib")

endif()
