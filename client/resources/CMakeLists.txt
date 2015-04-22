# we don't want to copy if we're building in the source dir
if (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    file (GLOB client_RESOURCE_FOLDERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")
    # Don't include the CMakeLists.txt
    list(REMOVE_ITEM client_RESOURCE_FOLDERS "CMakeLists.txt")

    foreach(item IN LISTS client_RESOURCE_FOLDERS)
        add_custom_command(
            OUTPUT "${CMAKE_BINARY_DIR}/bin/client/${item}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
                "${CMAKE_BINARY_DIR}/bin/client/${item}"
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
        )
    endforeach()

    add_custom_target(client-resource ALL DEPENDS ${client_RESOURCE_FOLDERS})
endif()
