cmake_minimum_required (VERSION 3.2 FATAL_ERROR)

project (AGS-Game-Project)
set (AGS-Game-Project_VERSION_MAJOR 0)
set (AGS-Game-Project_VERSION_MINOR 0)
set (AGS-Game-Project_VERSION_PATCH 1)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include ("${CMAKE_SOURCE_DIR}/cmake/internal_utils.cmake")

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)
include_directories ("${PROJECT_BINARY_DIR}")

option(BUILD_UNIT_TESTS "Build Unit Tests." OFF)
if (BUILD_UNIT_TESTS)
    enable_testing()
endif (BUILD_UNIT_TESTS)

find_package (OpenGL REQUIRED)
find_package (GLEW REQUIRED)
find_package (GLFW REQUIRED)
find_package (GLM REQUIRED)
#find_package (FreeImage REQUIRED)
find_package (GTest REQUIRED)
find_package (LibRT) # For liquidfun unit tests on Linux

add_subdirectory (client)
