#
# Create an executable test from a source file. The associate executable has the same
# name as the source file.
#
# Usage:
#   add_gtest_test(SOURCE <source>
#                [INCLUDE [items1...] ]
#                [LINK [items1...]
#                [PREFIX <string>]
#                [COMPILE_OPTIONS [items1...]
#                [COMPILE_DEFINITIONS [items1...]] )
#
# Parameters:
#
# SOURCE is the source file containing the test
# INCLUDE is an optional list of include directories
# LINK is an optional list of targets/libraries to link with
# PREFIX is an optional string to append to the name of the test (e.g. the module name)
#     For example if foo.cpp is passed as source, and "bar" as PREFIX the executable
#     will be named named "bar_foo". Without PREFIX it will be just "foo"
# COMPILE_OPTIONS is an optional list of compile options to add to the target
# COMPILE_DEFINITIONS is an optional list of compile definitions to add to the target
#
# The function parses each test file and extract the BOOST_AUTO_TEST_SUITEs so
# that each suite is run and visualised as a separated test. This means that no
# BOOST_AUTO_TEST_CASE must be left outside the suite or it will not be run during make test
#
# Function based on https://eb2.co/blog/2015/06/driving-boost-dot-test-with-cmake/
#
function(add_gtest_test)

    set(options OPTIONS)
    set(oneValueArgs SOURCE PREFIX)
    set(multiValueArgs INCLUDE LINK)
    cmake_parse_arguments(param "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

#    message(STATUS "INCLUDE ${param_INCLUDE}")
#    message(STATUS "LINK ${param_LINK}")
#    message(STATUS "PREFIX ${param_PREFIX}")
#    message(STATUS "SOURCE ${param_SOURCE}")

    # get the name without the extension (NAME_WE)
    get_filename_component(TEST_EXECUTABLE_NAME ${param_SOURCE} NAME_WE)

    if(NOT ${param_PREFIX} STREQUAL "")
        set(TEST_EXECUTABLE_NAME "${param_PREFIX}_${TEST_EXECUTABLE_NAME}")
    endif()

    add_executable(${TEST_EXECUTABLE_NAME} ${param_SOURCE})

    if(NOT ${param_INCLUDE} STREQUAL "")
        target_include_directories(${TEST_EXECUTABLE_NAME} PUBLIC ${param_INCLUDE})
    endif()

    target_link_libraries(${TEST_EXECUTABLE_NAME}
            ${param_LINK} GTest::gtest_main)

    if(param_COMPILE_OPTIONS)
        target_compile_options( ${TEST_EXECUTABLE_NAME} PUBLIC ${param_COMPILE_OPTIONS})
    endif()

    if(param_COMPILE_DEFINITIONS)
        target_compile_definitions( ${TEST_EXECUTABLE_NAME} PUBLIC ${param_COMPILE_DEFINITIONS})
    endif()

    # Discover and register the tests
    include(GoogleTest)
    gtest_discover_tests(${TEST_EXECUTABLE_NAME} PREFIX ${param_PREFIX})

#    add_test(NAME ${TEST_EXECUTABLE_NAME}
#             COMMAND ${TEST_EXECUTABLE_NAME} --catch_system_error=yes)

endfunction()