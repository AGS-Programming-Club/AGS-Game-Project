# FindGLFW
# --------
# Defines:
# GLFW_FOUND - true if GLFW has been found and can be used
# GLFW_INCLUDE_DIRS - include directories for GLFW
# GLFW_LIBRARIES - libraries to link against GLFW



set (GLFW_ROOT_DIR "${PROJECT_SOURCE_DIR}/external/glfw-3.1.1")

add_subdirectory (${GLFW_ROOT_DIR} glfw EXCLUDE_FROM_ALL)

set (GLFW_FOUND true)
set (GLFW_INCLUDE_DIRS "${GLFW_ROOT_DIR}/include")
set (GLFW_LIBRARIES glfw ${GLFW_LIBRARIES})
