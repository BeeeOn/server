# firehazard CMake
# Marek Beňo 2016

cmake_minimum_required (VERSION 2.8)

project (firehazard)

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../lib/modules/)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -pedantic -Wextra -std=c++11")

set (FIREHAZARD_SOURCES
	src/FireHazardInstance.cpp
	src/FireHazardManager.cpp
	)

link_directories("/usr/local/lib64" "/usr/pgsql-9.5/lib")
include_directories("/usr/include/soci" "/usr/pgsql-9.5/include" "../lib" "/usr/local")

find_package(Soci COMPONENTS postgresql REQUIRED)
find_package (Threads)
find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_UTIL PocoUtil)

add_library (${PROJECT_NAME} SHARED ${FIREHAZARD_SOURCES})

