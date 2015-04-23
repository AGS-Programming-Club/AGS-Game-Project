# FindGLM
# --------
# Defines:
# GLM_FOUND - true if GLM has been found and can be used
# GLM_INCLUDE_DIRS - include directories for GLM



if (WIN32)
    set (GLM_INCLUDE_SEARCH_DIRS "${PROJECT_SOURCE_DIR}/external/glm/include/")
endif (WIN32)

find_path (GLM_INCLUDE_DIR
    NAMES glm/glm.hpp
    HINTS ${GLM_INCLUDE_SEARCH_DIRS})

set (GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})

include ("${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake")
find_package_handle_standard_args (GLM
                                   REQUIRED_VARS GLM_INCLUDE_DIR)

mark_as_advanced(GLM_INCLUDE_DIR)
