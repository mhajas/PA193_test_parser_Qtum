project(PA193_QTUM_parser)
cmake_minimum_required(VERSION 3.8)

add_subdirectory(parser)

add_executable(parser-main main.cpp)

target_link_libraries(parser-main parser-lib)

option(BUILD_testsuite "Build all tests." ON)

if (BUILD_testsuite)
    add_subdirectory(testsuite)
endif()
