set (client_LIBRARIES ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})

# Hack around ${OPENGL_INCLUDE_DIR} being empty under Windows
if (NOT "${OPENGL_INCLUDE_DIR}" STREQUAL "")
    include_directories ("${OPENGL_INCLUDE_DIR}")
endif (NOT "${OPENGL_INCLUDE_DIR}" STREQUAL "")
include_directories ("${GLEW_INCLUDE_DIRS}")
include_directories ("${GLFW_INCLUDE_DIR}")
include_directories ("${GLM_INCLUDE_DIRS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/client")

include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/include/")

add_subdirectory (src)

if (BUILD_UNIT_TESTS)
    add_subdirectory (test)
endif (BUILD_UNIT_TESTS)

add_subdirectory (resources)
