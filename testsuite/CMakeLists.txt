find_package(Git)
if (NOT EXISTS googletest/CMakeLists.txt)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()

add_subdirectory(googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(testsuite test-main.cpp
        parsing_block_tests.cpp
        parse_test_json.cpp
        external/json.hpp
        )

target_link_libraries(testsuite gtest gtest_main parser-lib)

target_include_directories(testsuite INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/..
        )

file(COPY test-resources DESTINATION resources)
