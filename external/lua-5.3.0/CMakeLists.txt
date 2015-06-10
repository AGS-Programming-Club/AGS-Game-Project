project (lua)
cmake_minimum_required (VERSION 2.6)

file (GLOB lua_SOURCE_FILES "src/*.c")
list (REMOVE_ITEM lua_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/lua.c")
list (REMOVE_ITEM lua_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/luac.c")

add_library (lua STATIC ${lua_SOURCE_FILES})
