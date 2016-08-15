# Baf CMake
# Marek Beňo 2016

include_directories(
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
)

cmake_minimum_required (VERSION 2.8)
project (server C CXX)

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -lpthread -ggdb3")
add_definitions(-std=c++11)

add_subdirectory(AliveCheck)
add_subdirectory(FireHazard)
add_subdirectory(Watchdog)
