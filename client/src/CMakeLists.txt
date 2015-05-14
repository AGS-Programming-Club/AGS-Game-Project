set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/client")

file (GLOB_RECURSE client_SOURCE_FILES "*.cpp")
set (client_SOURCE_FILES ${client_SOURCE_FILES} PARENT_SCOPE)

add_executable (client ${client_SOURCE_FILES} "../main.cpp")
target_link_libraries (client ${client_LIBRARIES})

if (WIN32)
    set (client_DLLS ${client_LIBRARIES})
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
