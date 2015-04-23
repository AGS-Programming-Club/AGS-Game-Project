set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/client")

include_directories ("${PROJECT_SOURCE_DIR}/client/include/")

add_subdirectory (src)
add_subdirectory (test)
add_subdirectory (resources)
