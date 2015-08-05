# Enable warning all for gcc or use /W4 /WX for visual studio

if(CMAKE_BUILD_TOOL MATCHES "(MSBuild|msdev|devenv|nmake)")

  set(CMAKE_CXX_WARNING_LEVEL 4)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX /wd4100 /wd4127 /wd4250 /wd4702 /wd4996")

  set(CMAKE_C_WARNING_LEVEL 4)
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  endif()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /WX /wd4100 /wd4127 /wd4250 /wd4702 /wd4996")

elseif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUC)

  if(NOT CMAKE_CXX_FLAGS MATCHES "-Wall$")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Werror")
  endif()

  if(NOT CMAKE_C_FLAGS MATCHES "-Wall$")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Werror")
  endif()

else()
  message(STATUS "Unknown build tool, cannot set compiler warning flags!")
endif()
