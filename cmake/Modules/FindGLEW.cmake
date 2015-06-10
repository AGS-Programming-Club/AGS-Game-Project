# FindGLEW
# --------
# Defines:
# GLEW_FOUND - true if GLEW has been found and can be used
# GLEW_INCLUDE_DIRS - include directories for GLEW
# GLEW_LIBRARIES - libraries to link against GLEW



set (GLEW_ROOT_DIR "${PROJECT_SOURCE_DIR}/external/glew-1.12.0")

add_subdirectory (${GLEW_ROOT_DIR} glew EXCLUDE_FROM_ALL)

set (GLEW_FOUND true)
set (GLEW_INCLUDE_DIRS "${GLEW_ROOT_DIR}/include")
set (GLEW_LIBRARIES GLEW_static)
