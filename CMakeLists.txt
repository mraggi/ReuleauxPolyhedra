cmake_minimum_required(VERSION 3.0)
project(reuleaux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_BUILD_TYPE Release)

add_definitions(-Ofast)

add_executable(cspsolver CSPSolver.cpp CSPSolvermain.cpp)
add_executable(embedding Embedding.cpp)
add_executable(to_stl_format to_scad_format.cpp)
