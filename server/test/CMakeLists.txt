cmake_minimum_required (VERSION 2.8.11)
project (server-test C CXX)

find_library (CPP_UNIT cppunit)
find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_UTIL PocoUtil)
find_library (POCO_NET PocoNet)
find_library (POCO_JSON PocoJSON)

file(GLOB TEST_SOURCES
	${PROJECT_SOURCE_DIR}/server/RouteTest.cpp
	${PROJECT_SOURCE_DIR}/server/SessionManagerTest.cpp
	${PROJECT_SOURCE_DIR}/di/DependencyInjectorTest.cpp
	${PROJECT_SOURCE_DIR}/model/EntityTest.cpp
	${PROJECT_SOURCE_DIR}/model/EntityCollectionTest.cpp
	${PROJECT_SOURCE_DIR}/model/SerializerTest.cpp
	${PROJECT_SOURCE_DIR}/service/AuthServiceTest.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
)

add_executable(test-suite test.cpp ${TEST_SOURCES})
target_link_libraries(test-suite
	-Wl,--whole-archive
	BeeeOnServer
	-Wl,--no-whole-archive
	${POCO_FOUNDATION}
	${POCO_UTIL}
	${POCO_NET}
	${POCO_JSON}
	mongoose
	${CPP_UNIT}
)
