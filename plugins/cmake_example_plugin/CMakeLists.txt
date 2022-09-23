cmake_minimum_required(VERSION 3.2)

include("${DEVKITPRO}/wups/share/wups.cmake" REQUIRED)

project(example_cpp CXX)

add_executable(example_cpp src/main.cpp)
wups_create_plugin(example_cpp)
