cmake_minimum_required(VERSION 2.6)
project(reuleaux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(CMAKE_BUILD_TYPE Debug)

add_executable(reuleaux main.cpp BuildBipartite.cpp)
add_executable(embedding Embedding.cpp)
