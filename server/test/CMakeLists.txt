cmake_minimum_required (VERSION 2.8.11)
project (server-test C CXX)

find_library (CPP_UNIT cppunit)

file(GLOB TEST_SOURCES
)

include_directories(
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
)

add_executable(test-suite test.cpp ${TEST_SOURCES})
target_link_libraries(test-suite
	${CPP_UNIT}
)
