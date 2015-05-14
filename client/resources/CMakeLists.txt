file (GLOB client_RESOURCE_FOLDERS "*")
foreach (item IN LISTS client_RESOURCE_FOLDERS)
    if (NOT IS_DIRECTORY ${item})
        list (REMOVE_ITEM client_RESOURCE_FOLDERS ${item})
    endif ()
endforeach ()

add_copy_directory_target (client-resource
    "${CMAKE_BINARY_DIR}/bin/client" ${client_RESOURCE_FOLDERS})
