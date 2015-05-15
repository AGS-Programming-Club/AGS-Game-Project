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
endforeach ()

add_library (client_test STATIC ${client_SOURCE_FILES} ${client_TEST_SUPPORT_SOURCE_FILES})

set (client_test_LIBRARIES ${GTEST_LIBRARIES})
if (LIBRT_FOUND)
    set (client_test_LIBRARIES ${client_test_LIBRARIES} ${LIBRT_LIBRARIES})
endif ()

foreach (src ${client_TEST_SOURCE_FILES})
    get_filename_component (name "${src}" NAME_WE)
    get_filename_component (dir "${src}" DIRECTORY)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${RUNTIME_OUTPUT_DIRECTORY_BASE}/${dir}")
    add_executable ("${name}" "${CMAKE_CURRENT_SOURCE_DIR}/${src}")
    target_link_libraries ("${name}" client_test ${client_LIBRARIES} ${client_test_LIBRARIES})
    add_test ("${name}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${name}")
endforeach ()

add_copy_directory_target (box2d-test-baselines
    "${RUNTIME_OUTPUT_DIRECTORY_BASE}/Box2D" "${CMAKE_CURRENT_SOURCE_DIR}/Box2D/baselines")
