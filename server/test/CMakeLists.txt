cmake_minimum_required (VERSION 2.8.11)
project (server-test C CXX)

find_library (CPP_UNIT cppunit)
find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_UTIL PocoUtil)
find_library (POCO_SSL PocoNetSSL)
find_library (POCO_CRYPTO PocoCrypto)
find_library (POCO_NET PocoNet)
find_library (POCO_JSON PocoJSON)
find_library (POCO_DATA PocoData)

file(GLOB TEST_SOURCES
	${PROJECT_SOURCE_DIR}/model/DevicePropertyTest.cpp
	${PROJECT_SOURCE_DIR}/server/SessionManagerTest.cpp
	${PROJECT_SOURCE_DIR}/service/AuthServiceTest.cpp
	${PROJECT_SOURCE_DIR}/provider/GoogleAuthProviderTest.cpp
	${PROJECT_SOURCE_DIR}/provider/FacebookAuthProviderTest.cpp
	${PROJECT_SOURCE_DIR}/rest/UriPatternTest.cpp
	${PROJECT_SOURCE_DIR}/rpc/LegacyGatewayRPCTest.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/base/src
	${CMAKE_SOURCE_DIR}/base/test
)

add_executable(test-suite-server
	${CMAKE_SOURCE_DIR}/base/test/test.cpp
	${TEST_SOURCES}
)
add_test(COMMAND test-suite-server)

set(LIBS
	${POCO_FOUNDATION}
	${POCO_SSL}
	${POCO_CRYPTO}
	${POCO_UTIL}
	${POCO_NET}
	${POCO_JSON}
	${POCO_DATA}
	${POCO_XML}
	${CPP_UNIT}
)

# Apple's linker doesn't support --whole-archive. Instead it uses -all_load.
if (APPLE)
	target_link_libraries(test-suite-server
		-Wl,-all_load
		BeeeOnBaseTest
		BeeeOnBase
		BeeeOnWork
		BeeeOnServer
		${LIBS}
	)
# The -Wl,--whole-archive is necessary for all libraries defining
# BEEEON_OBJECTs (see di/Injectable.h).
else()
	target_link_libraries(test-suite-server
		-Wl,--whole-archive
		BeeeOnBaseTest
		BeeeOnBase
		BeeeOnWork
		BeeeOnServer
		-Wl,--no-whole-archive
		${LIBS}
	)
endif()

install(TARGETS test-suite-server
	RUNTIME DESTINATION share/beeeon/test-suite
	CONFIGURATIONS Debug
)
