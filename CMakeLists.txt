project(PA193_QTUM_parser)
cmake_minimum_required(VERSION 3.8)

set (CMAKE_CXX_STANDARD 14)
if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    add_compile_options(-Wall -Wextra)
elseif (MSVC AND (MSVC_VERSION GREATER 1700))
    add_compile_options(/GS /WALL /W4)
    # nothing special yet
else()
    message(FATAL_ERROR "unsuported compiler id:${CMAKE_CXX_COMPILER_ID}, path: ${CMAKE_CXX_COMPILER}")
endif()

add_subdirectory(parser)

add_executable(parser-main main.cpp)

target_link_libraries(parser-main parser-lib)

option(BUILD_testsuite "Build all tests." ON)

if (BUILD_testsuite)
    add_subdirectory(testsuite)
endif()
