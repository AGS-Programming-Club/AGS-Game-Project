set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/client")

file (GLOB_RECURSE client_SOURCE_FILES "*.cpp")
set (client_SOURCE_FILES ${client_SOURCE_FILES} PARENT_SCOPE)

add_executable (client ${client_SOURCE_FILES} "../main.cpp")
target_link_libraries (client ${client_LIBRARIES})
