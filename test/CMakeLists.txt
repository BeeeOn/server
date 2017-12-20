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
find_library (POCO_ODBC PocoDataODBC)
find_library (ICU_I18N icui18n)
find_library (ICU_UC icuuc)
find_library (JWT jwt)
find_library (JANSSON jansson)

file(GLOB TEST_SOURCES
	${PROJECT_SOURCE_DIR}/fcm/FCMSenderTest.cpp
	${PROJECT_SOURCE_DIR}/fcm/LegacyFCMClientTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/HTTPLocaleExtractorTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuLocaleImplTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuLocaleManagerTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuTranslatorTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuTimeZoneImplTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/TimeZoneImplTest.cpp
	${PROJECT_SOURCE_DIR}/model/DevicePropertyTest.cpp
	${PROJECT_SOURCE_DIR}/server/CORSFilterTest.cpp
	${PROJECT_SOURCE_DIR}/server/SessionManagerTest.cpp
	${PROJECT_SOURCE_DIR}/server/TestingHTTPServerRequest.cpp
	${PROJECT_SOURCE_DIR}/server/TestingHTTPServerResponse.cpp
	${PROJECT_SOURCE_DIR}/service/AuthServiceImplTest.cpp
	${PROJECT_SOURCE_DIR}/provider/GoogleAuthProviderTest.cpp
	${PROJECT_SOURCE_DIR}/provider/FacebookAuthProviderTest.cpp
	${PROJECT_SOURCE_DIR}/rest/UriPatternTest.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchPlanTest.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchPlanParserTest.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchRefTest.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchTestingPlans.cpp
	${PROJECT_SOURCE_DIR}/util/DevicesSAXHandlerTest.cpp
	${PROJECT_SOURCE_DIR}/util/SanitizeTest.cpp
	${PROJECT_SOURCE_DIR}/util/TypesSAXHandlerTest.cpp
)

if (JWT AND JANSSON)
	file(GLOB JWT_TEST_SOURCES
		${PROJECT_SOURCE_DIR}/jwt/JWTConstants.cpp
		${PROJECT_SOURCE_DIR}/jwt/JWTDecoderTest.cpp
		${PROJECT_SOURCE_DIR}/jwt/JWTEncoderTest.cpp
		${PROJECT_SOURCE_DIR}/server/JWTSessionManagerTest.cpp
	)
	list(APPEND TEST_SOURCES ${JWT_TEST_SOURCES})
endif()

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

exec_program(
	"genrb"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS
	-d l10n/test
	l10n/test/res_index.txt
	l10n/test/root.txt
	l10n/test/en.txt
	l10n/test/cs.txt
)

set(LIBS
	${POCO_FOUNDATION}
	${POCO_SSL}
	${POCO_CRYPTO}
	${POCO_UTIL}
	${POCO_NET}
	${POCO_JSON}
	${POCO_DATA}
	${POCO_XML}
	${ICU_I18N}
	${ICU_UC}
	${CPP_UNIT}
)

if(JANSSON)
	list(APPEND LIBS ${JANSSON})
endif()

if(JWT)
	list(APPEND LIBS ${JWT})
endif()

# Apple's linker doesn't support --whole-archive. Instead it uses -all_load.
if (APPLE)
	target_link_libraries(test-suite-server
		-Wl,-all_load
		BeeeOnBaseTest
		BeeeOnBase
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
		BeeeOnServer
		-Wl,--no-whole-archive
		${LIBS}
	)
endif()

if(POCO_ODBC)
	target_link_libraries(test-suite-server ${POCO_ODBC})
endif()
