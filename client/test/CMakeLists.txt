include_directories ("${GTEST_INCLUDE_DIRS}")
add_definitions (-DLIQUIDFUN_UNIT_TESTS=1)
add_definitions (-DUNIT_TESTS=1)

set (RUNTIME_OUTPUT_DIRECTORY_BASE "${CMAKE_BINARY_DIR}/test/client")

file (GLOB_RECURSE client_TEST_SOURCE_FILES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*Tests.cpp")
file (GLOB_RECURSE client_TEST_SUPPORT_SOURCE_FILES "*.cpp")
foreach (src ${client_TEST_SUPPORT_SOURCE_FILES})
    if ("${src}" MATCHES ".*Tests.cpp")
        list (REMOVE_ITEM client_TEST_SUPPORT_SOURCE_FILES "${src}")
    endif ("${src}" MATCHES ".*Tests.cpp")
endforeach (src ${client_TEST_SUPPORT_SOURCE_FILES})

add_library (clienttest STATIC ${client_SOURCE_FILES} ${client_TEST_SUPPORT_SOURCE_FILES})

foreach (src ${client_TEST_SOURCE_FILES})
    get_filename_component (name "${src}" NAME_WE)
    get_filename_component (dir "${src}" DIRECTORY)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_OUTPUT_DIRECTORY_BASE}/${dir}")
    add_executable ("${name}" "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    target_link_libraries ("${name}" clienttest ${client_LIBRARIES} ${GTEST_LIBRARIES})
    add_test ("${name}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${name}")
endforeach (src ${client_TEST_SOURCE_FILES})
