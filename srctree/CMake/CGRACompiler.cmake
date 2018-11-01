################################################################
#                          Created by:                         #
#                         James Miller                         #
################################################################

# using Clang or GCC
if(${CMAKE_CXX_COMPILER_ID} MATCHES "^(Apple)?Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")

  # General options to pass to both C & C++ compilers
  set(GENERAL_OPTIONS)

  # General options to pass to the C++ compiler
  set(GENERAL_CXX_OPTIONS)
  list(APPEND GENERAL_CXX_OPTIONS
    # C++14
    "std=c++1y"
    # Don't allow use of names for operators
    "fno-operator-names"
    # Standard warning set
    "Wall" "Wextra" "pedantic"
    # Warn on overloaded virtual functions, this is normally
    # a mistake
    "Woverloaded-virtual"
    # Turn format-string warnings into errors
    "Werror=format-security"
    # Turn warnings about incorrect return types into errors
    "Werror=return-type"
  )

  # Enable coloured output for GCC 4.9 or greater
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9)
      list(APPEND GENERAL_CXX_OPTIONS "fdiagnostics-color")
    endif()
  endif()

  # Options to pass for debug mode to the C++ compiler
  set(DEBUG_CXX_OPTIONS)

  # Options to pass for release mode to the C++ compiler
  set(RELEASE_CXX_OPTIONS)

  set(CMAKE_C_FLAGS_DEBUG            "-O0 -g")
  set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
  set(CMAKE_C_FLAGS_MINSIZEREL       "-Os -DNDEBUG")
  set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
  set(CMAKE_C_FLAGS_RELEASE          "-O3 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -DNDEBUG")
  set(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O2 -g")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
  set(CMAKE_C_FLAGS                  "${CMAKE_C_FLAGS} -w")

  foreach(opt ${DISABLED_NAMED_WARNINGS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-${opt}")
  endforeach()

  foreach(opt ${GENERAL_OPTIONS})
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -${opt}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -${opt}")
  endforeach()

  foreach(opt ${GENERAL_CXX_OPTIONS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -${opt}")
  endforeach()

  foreach(opt ${DEBUG_CXX_OPTIONS})
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -${opt}")
  endforeach()

  foreach(opt ${RELEASE_CXX_OPTIONS})
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -${opt}")
  endforeach()
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  # C++ Latest
  add_compile_options(/std:c++latest)
  # UTF-8 source and execution charsets
  add_compile_options(/utf-8)
  # Full warning and multithreaded builds
  add_compile_options(/W4 /MP)
  # Disable C4800: forcing X to bool (performance warning)
  add_compile_options(/wd4800)
endif()