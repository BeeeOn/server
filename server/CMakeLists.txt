cmake_minimum_required (VERSION 2.8.11)
project (server C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib/modules)

find_package(Soci COMPONENTS postgresql sqlite3 REQUIRED)

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -pedantic -Wall -lpthread -ggdb3")
add_definitions(-std=c++11)

add_subdirectory(lib/mongoose)
include_directories(lib/mongoose)

add_subdirectory(src)
add_subdirectory(test)
