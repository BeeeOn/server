cmake_minimum_required (VERSION 2.8.11)
project (server-test C CXX)

find_library (CPP_UNIT cppunit)
find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_UTIL PocoUtil)
find_library (POCO_SSL PocoNetSSL)
find_library (POCO_CRYPTO PocoCrypto)
find_library (POCO_NET PocoNet)
find_library (POCO_JSON PocoJSON)
find_library (POCO_XML PocoXML)
find_library (POCO_DATA PocoData)

file(GLOB TEST_BASE_SOURCES
	${PROJECT_SOURCE_DIR}/DebugTest.cpp
	${PROJECT_SOURCE_DIR}/cppunit/TapTestProducer.cpp
	${PROJECT_SOURCE_DIR}/di/DependencyInjectorTest.cpp
	${PROJECT_SOURCE_DIR}/di/DIWrapperTest.cpp
	${PROJECT_SOURCE_DIR}/model/GatewayIDTest.cpp
	${PROJECT_SOURCE_DIR}/model/DeviceIDTest.cpp
	${PROJECT_SOURCE_DIR}/util/DAMMTest.cpp
	${PROJECT_SOURCE_DIR}/util/Base64Test.cpp
	${PROJECT_SOURCE_DIR}/util/SecureXmlParserTest.cpp
	${PROJECT_SOURCE_DIR}/util/TypesSAXHandlerTest.cpp
	${PROJECT_SOURCE_DIR}/util/DevicesSAXHandlerTest.cpp
	${PROJECT_SOURCE_DIR}/ssl/X509FingerprintTest.cpp
)

file(GLOB TEST_SOURCES
	${PROJECT_SOURCE_DIR}/server/RouteTest.cpp
	${PROJECT_SOURCE_DIR}/server/SessionManagerTest.cpp
	${PROJECT_SOURCE_DIR}/service/AuthServiceTest.cpp
	${PROJECT_SOURCE_DIR}/provider/GoogleAuthProviderTest.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/src
)

add_executable(test-suite test.cpp ${TEST_SOURCES} ${TEST_BASE_SOURCES})

set(LIBS
	${POCO_FOUNDATION}
	${POCO_SSL}
	${POCO_CRYPTO}
	${POCO_UTIL}
	${POCO_NET}
	${POCO_JSON}
	${POCO_XML}
	${POCO_DATA}
	${CPP_UNIT}
)

# Apple's linker doesn't support --whole-archive. Instead it uses -all_load.
if (APPLE)
	target_link_libraries(test-suite
		-Wl,-all_load
		BeeeOnBase
		BeeeOnServer
		${LIBS}
	)
# The -Wl,--whole-archive is necessary for all libraries defining
# BEEEON_OBJECTs (see di/Injectable.h).
else()
	target_link_libraries(test-suite
		-Wl,--whole-archive
		BeeeOnBase
		BeeeOnServer
		-Wl,--no-whole-archive
		${LIBS}
	)
endif()

install(TARGETS test-suite-server
	RUNTIME DESTINATION usr/share/beeeon/test-suite
	CONFIGURATIONS Debug
)
