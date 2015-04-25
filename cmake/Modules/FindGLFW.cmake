# FindGLFW
# --------
# Defines:
# GLFW_FOUND - true if GLFW has been found and can be used
# GLFW_INCLUDE_DIRS - include directories for GLFW
# GLFW_LIBRARIES - libraries to link against GLFW



unset (GLFW_INCLUDE_SEARCH_DIRS)
unset (GLFW_LIBRARY_SEARCH_DIRS)

if (WIN32)
    set (GLFW_ROOT_PATH "${PROJECT_SOURCE_DIR}/external/glfw")

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bit system
        set (GLFW_ROOT_PATH "${GLFW_ROOT_PATH}/WIN64")
    else (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 32 bit system
        set (GLFW_ROOT_PATH "${GLFW_ROOT_PATH}/WIN32")
    endif (CMAKE_SIZEOF_VOID_P EQUAL 8)

    set (GLFW_INCLUDE_SEARCH_DIRS "${GLFW_ROOT_PATH}/include")

    if (MINGW)
        set (GLFW_LIBRARY_SEARCH_DIRS "${GLFW_ROOT_PATH}/lib-mingw")
    else (MINGW)
        set (FATAL_ERROR "Unsupported compiler!")
    endif (MINGW)
endif (WIN32)

message (${GLFW_LIBRARY_SEARCH_DIRS})

find_path (GLFW_INCLUDE_DIR
    NAMES GLFW/glfw3.h
    HINTS ${GLFW_INCLUDE_SEARCH_DIRS})

find_library (GLFW_LIBRARY
    NAMES glfw glfw3
    HINTS ${GLFW_LIBRARY_SEARCH_DIRS}
    PATH_SUFFIXES lib64)

set (GLFW_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
set (GLFW_LIBRARIES ${GLFW_LIBRARY})

include ("${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake")
find_package_handle_standard_args (GLFW
                                   REQUIRED_VARS GLFW_INCLUDE_DIR GLFW_LIBRARY)

mark_as_advanced(GLFW_INCLUDE_DIR GLFW_LIBRARY)
