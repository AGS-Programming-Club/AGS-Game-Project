include_directories ("${PROJECT_SOURCE_DIR}/client/include/")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/client")

add_subdirectory (src)
add_subdirectory (test)
add_subdirectory (resources)
