cmake_minimum_required (VERSION 2.8.11)
project (server C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib/modules)

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -pedantic -Wall -lpthread -ggdb3")
add_definitions(-std=c++11)

if(DEFINED ENV{GIT_ID})
	set (GIT_ID $ENV{GIT_ID})
else()
	exec_program(
		"git"
		${CMAKE_CURRENT_SOURCE_DIR}
		ARGS "describe --always --tags"
		OUTPUT_VARIABLE GIT_ID)
endif()

add_definitions(-DGIT_ID="${GIT_ID}")

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(schema)
