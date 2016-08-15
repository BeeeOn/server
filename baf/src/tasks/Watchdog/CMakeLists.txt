# watchdog CMake
# Marek Beňo 2016

cmake_minimum_required (VERSION 2.8)

project (watchdog)

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../../lib/modules/)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -pedantic -Wextra -std=c++11")

set (WATCHDOG_SOURCES
	src/WatchdogInstance.cpp
	src/WatchdogManager.cpp
	)

include_directories("../lib" "/usr/local")

find_package(Soci COMPONENTS postgresql REQUIRED)
find_package (Threads)
find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_UTIL PocoUtil)

add_library (${PROJECT_NAME} SHARED ${WATCHDOG_SOURCES})

