project("affdexCrash" C CXX)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_C_COMPILER "/usr/bin/gcc-5")
set(CMAKE_CXX_COMPILER "/usr/bin/g++-5")

add_subdirectory("src")
add_subdirectory("test")
