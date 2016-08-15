# Baf CMake
# Marek Beňo 2016

cmake_minimum_required (VERSION 2.8)
project (baf)

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -lpthread -ggdb3")
add_definitions(-std=c++11)

add_subdirectory(src)
