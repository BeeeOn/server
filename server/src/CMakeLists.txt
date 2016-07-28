cmake_minimum_required (VERSION 2.8.11)
project (ui-server C CXX)

find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_UTIL PocoUtil)
find_library (POCO_NET PocoNet)

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -DBEEEON_SELECT_MONGOOSE")
add_definitions(-std=c++11)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/mongoose/MongooseResponse.cpp
	${PROJECT_SOURCE_DIR}/di/InjectorTarget.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}
)

add_library(BeeeOnServer ${SOURCES})

add_executable(ui-server ui/main.cpp ui/UIServerModule.cpp)

# The -Wl,--whole-archive is necessary for all libraries defining
# BEEEON_OBJECTs (see di/InjectorTarget.h).
target_link_libraries(ui-server
	-Wl,--whole-archive
	BeeeOnServer
	-Wl,--no-whole-archive
	${POCO_FOUNDATION}
	${POCO_UTIL}
	${POCO_NET}
	mongoose
)
