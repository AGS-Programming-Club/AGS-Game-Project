# FindLUA
# --------
# Defines:
# LUA_FOUND - true if Lua has been found and can be used
# LUA_INCLUDE_DIRS - include directories for Lua
# LUA_LIBRARIES - libraries to link against Lua



set (LUA_ROOT_DIR "${PROJECT_SOURCE_DIR}/external/lua-5.3.0")

add_subdirectory (${LUA_ROOT_DIR} lua EXCLUDE_FROM_ALL)

set (LUA_FOUND true)
set (LUA_INCLUDE_DIRS "${LUA_ROOT_DIR}/src")
set (LUA_LIBRARIES lua)
