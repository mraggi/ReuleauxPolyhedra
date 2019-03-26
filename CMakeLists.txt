cmake_minimum_required(VERSION 2.6)
project(reuleaux)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BUILD_TYPE Debug)

add_definitions(-Ofast)

add_executable(reuleaux main.cpp BuildBipartite.cpp)
