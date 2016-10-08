if(NOT TARGET zlib)

  # Temporary disable some warnings
  # C4127: conditional expression is constant
  # C4131: 'function' : uses old-style declarator
  # C4210: nonstandard extension used : function given file scope
  # C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
  if(CMAKE_MAKE_PROGRAM MATCHES "(MSBuild|devenv|msdev|nmake)")
    set(CMAKE_OLD_C_FLAGS ${CMAKE_C_FLAGS})
    set(CMAKE_OLD_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4127 /wd4131 /wd4210 /wd4244")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127 /wd4131 /wd4210 /wd4244")
  endif()

  # Set module options
  option(AMD64 "Enable building amd64 assembly implementation" ON)

  # Set install flag
  set(SKIP_INSTALL_ALL Y)

  # Add module subdirectory
  add_subdirectory("zlib")

  # Add missing target sources
  if(MSVC AND AMD64)
    target_sources(zlib PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/zlib/contrib/masmx64/inffas8664.c")
    target_sources(zlibstatic PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/zlib/contrib/masmx64/inffas8664.c")
  endif()

  # Set module folder
  set_target_properties(zlib zlibstatic example minigzip PROPERTIES FOLDER modules/zlib)

  # Exclude some modules from the build
  set_target_properties(example minigzip PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)

  # Restore default warnings
  if(CMAKE_MAKE_PROGRAM MATCHES "(MSBuild|devenv|msdev|nmake)")
    set(CMAKE_C_FLAGS ${CMAKE_OLD_C_FLAGS})
    set(CMAKE_CXX_FLAGS ${CMAKE_OLD_CXX_FLAGS})
  endif()

endif()
