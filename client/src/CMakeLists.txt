file (GLOB_RECURSE client_HEADER_FILES "*.hpp")
file (GLOB_RECURSE client_SOURCE_FILES "*.cpp")

add_executable (client
    ${client_HEADER_FILES} # Needed for eclipse
    ${client_SOURCE_FILES}
)



# Hack around ${OPENGL_INCLUDE_DIR} being empty under Windows
if (NOT "${OPENGL_INCLUDE_DIR}" STREQUAL "")
    include_directories ("${OPENGL_INCLUDE_DIR}")
endif (NOT "${OPENGL_INCLUDE_DIR}" STREQUAL "")
target_link_libraries (client ${OPENGL_LIBRARIES})

include_directories ("${GLEW_INCLUDE_DIRS}")
target_link_libraries (client ${GLEW_LIBRARIES})

include_directories ("${GLFW_INCLUDE_DIR}")
target_link_libraries (client ${GLFW_LIBRARIES})

include_directories ("${GLM_INCLUDE_DIRS}")

include_directories ("${FREEIMAGE_INCLUDE_DIR}")
target_link_libraries (client ${FREEIMAGE_LIBRARIES})



if (WIN32)
    set (client_DLLS ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})
    list (LENGTH client_DLLS client_DLLS_COUNT)
    math (EXPR client_DLLS_BOUND "${client_DLLS_COUNT} - 1")

    foreach (INDEX RANGE 0 ${client_DLLS_BOUND})
        list (GET client_DLLS ${INDEX} SOURCE)
        get_filename_component (FILENAME ${SOURCE} NAME)
		
        set (DESTINATION "${CMAKE_BINARY_DIR}/bin/client/${FILENAME}")

		# FIXME Hack for custom command not working
		configure_file (${SOURCE} ${DESTINATION} COPYONLY)

        #add_custom_command (
        #    OUTPUT "${DESTINATION}"
        #    COMMAND ${CMAKE_COMMAND} -E copy
        #        "${SOURCE}"
        #        "${DESTINATION}"
        #    DEPENDS "${SOURCE}"
        #)
    endforeach (INDEX RANGE 0 ${client_DLLS_BOUND})

    #add_custom_target (client-dlls ALL DEPENDS ${client_DLLS})
endif (WIN32)
