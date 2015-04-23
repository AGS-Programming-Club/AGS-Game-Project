# FindGLEW
# --------
# Defines:
# GLEW_FOUND - true if GLEW has been found and can be used
# GLEW_INCLUDE_DIRS - include directories for GLEW
# GLEW_LIBRARIES - libraries to link against GLEW



unset (GLEW_INCLUDE_SEARCH_DIRS)
unset (GLEW_LIBRARY_SEARCH_DIRS)

if (WIN32)
    set (GLEW_ROOT_PATH "${PROJECT_SOURCE_DIR}/external/glew")

    if (MINGW)
        set (GLEW_LIBRARY_PREFIX "${GLEW_ROOT_PATH}/bin/Release")
    elseif (MSVC)
        set (GLEW_LIBRARY_PREFIX "${GLEW_ROOT_PATH}/lib/Release")
    else (MINGW)
        set (FATAL_ERROR "Unknown compiler!")
    endif (MINGW)

    set (GLEW_INCLUDE_SEARCH_DIRS "${GLEW_ROOT_PATH}/include")

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bit system
        set (GLEW_LIBRARY_SEARCH_DIRS "${GLEW_LIBRARY_PREFIX}/x64")
    else (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 32 bit system
        set (GLEW_LIBRARY_SEARCH_DIRS "${GLEW_LIBRARY_PREFIX}/Win32")
    endif (CMAKE_SIZEOF_VOID_P EQUAL 8)
endif (WIN32)

find_path (GLEW_INCLUDE_DIR
    NAMES GL/glew.h
    HINTS ${GLEW_INCLUDE_SEARCH_DIRS})

find_library (GLEW_LIBRARY
    NAMES GLEW glew32 glew glew32s
    HINTS ${GLEW_LIBRARY_SEARCH_DIRS}
    PATH_SUFFIXES lib64)

set (GLEW_INCLUDE_DIRS ${GLEW_INCLUDE_DIR})
set (GLEW_LIBRARIES ${GLEW_LIBRARY})

include ("${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake")
find_package_handle_standard_args (GLEW
                                   REQUIRED_VARS GLEW_INCLUDE_DIR GLEW_LIBRARY)

mark_as_advanced(GLEW_INCLUDE_DIR GLEW_LIBRARY)
