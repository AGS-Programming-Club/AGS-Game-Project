# FindLIQUIDFUN
# --------
# Defines:
# LIQUIDFUN_FOUND - true if LIQUIDFUN has been found and can be used
# LIQUIDFUN_INCLUDE_DIRS - include directories for LIQUIDFUN
# LIQUIDFUN_LIBRARIES - libraries to link against LIQUIDFUN



unset (LIQUIDFUN_INCLUDE_SEARCH_DIRS)
unset (LIQUIDFUN_LIBRARY_SEARCH_DIRS)

if (WIN32)
    set (LIQUIDFUN_ROOT_PATH "${PROJECT_SOURCE_DIR}/external/liquidfun")

    if (MINGW)
        set (LIQUIDFUN_LIBRARY_PREFIX "${LIQUIDFUN_ROOT_PATH}/lib-mingw")
    else (MINGW)
        set (FATAL_ERROR "Unsupported compiler!")
    endif (MINGW)

    set (LIQUIDFUN_INCLUDE_SEARCH_DIRS "${LIQUIDFUN_ROOT_PATH}/include")

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bit system
        set (LIQUIDFUN_LIBRARY_SEARCH_DIRS "${LIQUIDFUN_LIBRARY_PREFIX}/x64")
    else (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 32 bit system
        set (LIQUIDFUN_LIBRARY_SEARCH_DIRS "${LIQUIDFUN_LIBRARY_PREFIX}/x86")
    endif (CMAKE_SIZEOF_VOID_P EQUAL 8)
endif (WIN32)

find_path (LIQUIDFUN_INCLUDE_DIR
    NAMES Box2D/Box2D.h
    HINTS ${LIQUIDFUN_INCLUDE_SEARCH_DIRS})

find_library (LIQUIDFUN_LIBRARY
    NAMES liquidfun
    HINTS ${LIQUIDFUN_LIBRARY_SEARCH_DIRS})

set (LIQUIDFUN_INCLUDE_DIRS ${LIQUIDFUN_INCLUDE_DIR})
set (LIQUIDFUN_LIBRARIES ${LIQUIDFUN_LIBRARY})

include ("${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake")
find_package_handle_standard_args (LiquidFun
                                   REQUIRED_VARS LIQUIDFUN_INCLUDE_DIR LIQUIDFUN_LIBRARY)

mark_as_advanced(LIQUIDFUN_INCLUDE_DIR LIQUIDFUN_LIBRARY)
