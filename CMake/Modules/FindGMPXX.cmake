macro(FIND_PACKAGE_HANDLE_STANDARD_ARGS _NAME _FAIL_MSG _VAR1 )

  if("${_FAIL_MSG}" STREQUAL "DEFAULT_MSG")
    set(_FAIL_MESSAGE "Could NOT find ${_NAME}")
  else()
    set(_FAIL_MESSAGE "${_FAIL_MSG}")
  endif()

  string(TOUPPER ${_NAME} _NAME_UPPER)

  set(${_NAME_UPPER}_FOUND TRUE)
  if(NOT ${_VAR1})
    set(${_NAME_UPPER}_FOUND FALSE)
  endif()

  foreach(_CURRENT_VAR ${ARGN})
    if(NOT ${_CURRENT_VAR})
      set(${_NAME_UPPER}_FOUND FALSE)
    endif()
  endforeach()

  if (${_NAME_UPPER}_FOUND)
    if (NOT ${_NAME}_FIND_QUIETLY)
      message(STATUS "Found ${_NAME}: ${${_VAR1}}")
    endif()
  else()
    if (${_NAME}_FIND_REQUIRED)
      message(FATAL_ERROR "${_FAIL_MESSAGE}")
    else()
      if (NOT ${_NAME}_FIND_QUIETLY)
        message(STATUS "${_FAIL_MESSAGE}")
      endif()
    endif()
  endif()

endmacro()

find_package( GMP QUIET )

if(GMP_FOUND)

  if (GMPXX_INCLUDE_DIR AND GMPXX_LIBRARIES)
    # Already in cache, be silent
    set(GMPXX_FIND_QUIETLY TRUE)
  endif()

  find_path(GMPXX_INCLUDE_DIR NAMES gmpxx.h
            PATHS ${GMP_INCLUDE_DIR_SEARCH}
            DOC "The directory containing the GMPXX include files"
           )

  find_library(GMPXX_LIBRARIES NAMES gmpxx
               PATHS ${GMP_LIBRARIES_DIR_SEARCH}
               DOC "Path to the GMPXX library"
               )

  find_package_handle_standard_args(GMPXX "DEFAULT_MSG" GMPXX_LIBRARIES GMPXX_INCLUDE_DIR )

endif()
