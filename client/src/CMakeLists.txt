file (GLOB_RECURSE client_HEADER_FILES "*.hpp")
file (GLOB_RECURSE client_SOURCE_FILES "*.cpp")

add_executable (client
    ${client_HEADER_FILES} # Needed for eclipse
    ${client_SOURCE_FILES}
)

find_package (OpenGL REQUIRED)
include_directories ("${OPENGL_INCLUDE_DIR}")
target_link_libraries (client ${OPENGL_LIBRARIES})

find_package (GLEW REQUIRED)
include_directories ("${GLEW_INCLUDE_DIRS}")
target_link_libraries (client ${GLEW_LIBRARIES})

find_package (GLFW REQUIRED)
include_directories ("${GLFW_INCLUDE_DIR}")
target_link_libraries (client ${GLFW_LIBRARIES})

# TODO Is GLM really required? Seems to build without it.
#find_package (GLM REQUIRED)
#include_directories ("${GLM_INCLUDE_DIRS}")
#target_link_libraries (client ${GLM_LIBRARIES})
