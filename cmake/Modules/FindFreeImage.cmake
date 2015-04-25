# FindFREEIMAGE
# --------
# Defines:
# FREEIMAGE_FOUND - true if FREEIMAGE has been found and can be used
# FREEIMAGE_INCLUDE_DIRS - include directories for FREEIMAGE
# FREEIMAGE_LIBRARIES - libraries to link against FREEIMAGE



unset (FREEIMAGE_INCLUDE_SEARCH_DIRS)
unset (FREEIMAGE_LIBRARY_SEARCH_DIRS)

if (WIN32)
    set (FREEIMAGE_ROOT_PATH "${PROJECT_SOURCE_DIR}/external/freeimage")

    if (MINGW)
        set (FREEIMAGE_LIBRARY_PREFIX "${FREEIMAGE_ROOT_PATH}/lib-mingw")
    else (MINGW)
        set (FATAL_ERROR "Unsupported compiler!")
    endif (MINGW)

    set (FREEIMAGE_INCLUDE_SEARCH_DIRS "${FREEIMAGE_ROOT_PATH}/include")

    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 64 bit system
        set (FREEIMAGE_LIBRARY_SEARCH_DIRS "${FREEIMAGE_LIBRARY_PREFIX}/x64")
    else (CMAKE_SIZEOF_VOID_P EQUAL 8)
        # 32 bit system
        set (FREEIMAGE_LIBRARY_SEARCH_DIRS "${FREEIMAGE_LIBRARY_PREFIX}/x86")
    endif (CMAKE_SIZEOF_VOID_P EQUAL 8)
endif (WIN32)

find_path (FREEIMAGE_INCLUDE_DIR
    NAMES FreeImagePlus.h
    HINTS ${FREEIMAGE_INCLUDE_SEARCH_DIRS})

find_library (FREEIMAGE_LIBRARY
    NAMES freeimageplus
    HINTS ${FREEIMAGE_LIBRARY_SEARCH_DIRS})

set (FREEIMAGE_INCLUDE_DIRS ${FREEIMAGE_INCLUDE_DIR})
set (FREEIMAGE_LIBRARIES ${FREEIMAGE_LIBRARY})

include ("${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake")
find_package_handle_standard_args (FreeImage
                                   REQUIRED_VARS FREEIMAGE_INCLUDE_DIR FREEIMAGE_LIBRARY)

mark_as_advanced(FREEIMAGE_INCLUDE_DIR FREEIMAGE_LIBRARY)
