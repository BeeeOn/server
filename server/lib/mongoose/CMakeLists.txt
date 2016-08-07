cmake_minimum_required (VERSION 2.8.11)
project (mongoose C)

set (CMAKE_C_FLAGS " ${CMAKE_C_FLAGS} -ggdb3")
add_definitions(-std=c99)

file(GLOB SOURCES
	mongoose.c
)

add_library(mongoose STATIC ${SOURCES})
