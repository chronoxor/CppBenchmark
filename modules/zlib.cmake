if(NOT TARGET zlib)

  # Assembler files
  if(CMAKE_COMPILER_IS_GNUCC)
    set(ASSEMBLER_FILES "contrib/amd64/amd64-match.S")
    add_definitions(-DASMV)
    set_source_files_properties(${ASSEMBLER_FILES} PROPERTIES LANGUAGE C COMPILE_FLAGS -DNO_UNDERLINE)
  elseif(MSVC)
    ENABLE_LANGUAGE(ASM_MASM)
    set(ASSEMBLER_FILES "zlib/contrib/masmx64/gvmat64.asm" "zlib/contrib/masmx64/inffasx64.asm")
    add_definitions(-DASMV -DASMINF)
    set(ASSEMBLER_FILES "zlib/contrib/masmx64/inffas8664.c" "zlib/contrib/masmx64/inffas8664.c")
  endif()

  # Module library
  file(SOURCE_FILES "zlib/*.c")
  if(CMAKE_MAKE_PROGRAM MATCHES "(MSBuild|devenv|msdev|nmake)")
    # C4127: conditional expression is constant
    # C4131: 'function' : uses old-style declarator
    # C4210: nonstandard extension used : function given file scope
    # C4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${COMMON_COMPILE_FLAGS} /wd4127 /wd4131 /wd4210 /wd4244")
  else()
    set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_FLAGS "${COMMON_COMPILE_FLAGS}")
  endif()
  add_library(zlib ${SOURCE_FILES} ${ASSEMBLER_FILES})
  target_link_libraries(zlib)

  # Module folder
  set_target_properties(zlib PROPERTIES FOLDER modules/zlib)

endif()
