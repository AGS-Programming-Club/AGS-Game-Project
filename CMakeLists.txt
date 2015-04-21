cmake_minimum_required (VERSION 2.8)

project (AGS-Game-Project)
set (AGS-Game-Project_VERSION_MAJOR 0)
set (AGS-Game-Project_VERSION_MINOR 0)
set (AGS-Game-Project_VERSION_PATCH 1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")


configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

include_directories("${PROJECT_BINARY_DIR}")


add_subdirectory (render)

file (GLOB main_HEADER_FILES "*.hpp")
file (GLOB main_SOURCE_FILES "*.cpp")

add_executable (game
    ${main_SOURCE_FILES}
    ${render_SOURCE_FILES}
)


find_package (OpenGL REQUIRED)
include_directories ("${OPENGL_INCLUDE_DIR}")
target_link_libraries (game ${OPENGL_LIBRARIES})

find_package (GLEW REQUIRED)
include_directories ("${GLEW_INCLUDE_DIRS}")
target_link_libraries (game ${GLEW_LIBRARIES})

find_package (GLFW REQUIRED)
include_directories ("${GLFW_INCLUDE_DIR}")
target_link_libraries (game ${GLFW_LIBRARIES})

# TODO Is GLM really required? Seems to build without it.
#find_package (GLM REQUIRED)
#include_directories ("${GLM_INCLUDE_DIRS}")
#target_link_libraries (game ${GLM_LIBRARIES})
