cmake_minimum_required (VERSION 2.8.11)
project (ui-server C CXX)

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS}")
add_definitions(-std=c++11)

file(GLOB SOURCES
)

include_directories(
	${PROJECT_SOURCE_DIR}
)

add_executable(ui-server ui/main.cpp)
target_link_libraries(ui-server
	${POCO_FOUNDATION}
	mongoose
)
