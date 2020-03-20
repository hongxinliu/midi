cmake_minimum_required(VERSION 3.0)
project(midi)

add_compile_options(-Wall -O3 -g -std=c++11 -fPIC)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(midi)
