cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 20)

include("$ENV{DEVKITPRO}/wups/share/wups.cmake" REQUIRED)

project(example_cmake C CXX)

if(DEFINED DEBUG)
    add_compile_definitions(DEBUG)
endif()

add_executable(example_cmake src/main.cpp)
add_subdirectory("src/utils")

wups_create_plugin(example_cmake)
