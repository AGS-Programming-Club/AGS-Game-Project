# add_copy_directory_target(name target_name dest_dir directories...)
#
# Creates a make target named "target_name"
# that copies each of the "directories" to "dest_dir".
# NOTE: "directories" be absolute paths without the trailing slash
function (add_copy_directory_target target_name dest_dir)
    add_custom_target ("${target_name}" ALL)
    foreach (dir IN LISTS ARGN)
        get_filename_component (name ${dir} NAME)
        add_custom_command(
            TARGET ${target_name}
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${dir}"
                "${dest_dir}/${name}")
    endforeach ()
endfunction ()
