cmake_minimum_required (VERSION 2.8.11)
project (server-test C CXX)

find_library (CPP_UNIT cppunit)
find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_UTIL PocoUtil)

file(GLOB TEST_SOURCES
	${PROJECT_SOURCE_DIR}/server/RouteTest.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
)

add_executable(test-suite test.cpp ${TEST_SOURCES})
target_link_libraries(test-suite
	${POCO_FOUNDATION}
	${POCO_UTIL}
	${CPP_UNIT}
)
