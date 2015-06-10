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

if (BUILD_SHARED_LIBS)
    message (FATAL_ERROR "BUILD_SHARED_LIBS must be OFF")
endif ()

if (NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Release"))
    set (CMAKE_BUILD_TYPE "Debug" CACHE STRING "Options are: Debug Release" FORCE)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions (-DDEBUG=1)
endif ()

option(BUILD_UNIT_TESTS "Build Unit Tests." OFF)
if (BUILD_UNIT_TESTS)
    enable_testing()
endif ()

configure_file (
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)
include_directories ("${PROJECT_BINARY_DIR}")

find_package (OpenGL REQUIRED)
find_package (GLEW REQUIRED)
find_package (GLFW REQUIRED)
find_package (GLM REQUIRED)
find_package (Lua REQUIRED)
if (BUILD_UNIT_TESTS)
    find_package (GTest REQUIRED)
    find_package (LibRT) # For liquidfun unit tests on Linux
endif ()

add_subdirectory (client)
