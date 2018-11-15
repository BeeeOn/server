cmake_minimum_required (VERSION 2.8.11)
project (server-test C CXX)

include(TargetLink)

find_library (CPP_UNIT cppunit)
find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_UTIL PocoUtil)
find_library (POCO_SSL PocoNetSSL)
find_library (POCO_CRYPTO PocoCrypto)
find_library (POCO_NET PocoNet)
find_library (POCO_JSON PocoJSON)
find_library (POCO_DATA PocoData)
find_library (ICU_I18N icui18n)
find_library (ICU_UC icuuc)
find_library (JWT jwt)
find_library (JANSSON jansson)
find_library (ZEROMQ zmq)

find_package(Lua "5.1" EXACT REQUIRED)

file(GLOB TEST_SOURCES
	${PROJECT_SOURCE_DIR}/dao/FilesystemQueryLoaderTest.cpp
	${PROJECT_SOURCE_DIR}/fcm/FCMSenderTest.cpp
	${PROJECT_SOURCE_DIR}/fcm/LegacyFCMClientTest.cpp
	${PROJECT_SOURCE_DIR}/gws/X509GatewayPeerVerifierTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/HTTPLocaleExtractorTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuLocaleImplTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuLocaleManagerTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuTranslatorTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuTimeZoneImplTest.cpp
	${PROJECT_SOURCE_DIR}/l10n/TimeZoneImplTest.cpp
	${PROJECT_SOURCE_DIR}/model/BitmapInfoTest.cpp
	${PROJECT_SOURCE_DIR}/model/DeviceTest.cpp
	${PROJECT_SOURCE_DIR}/model/DevicePropertyTest.cpp
	${PROJECT_SOURCE_DIR}/oauth2/FacebookOAuth2ProviderTest.cpp
	${PROJECT_SOURCE_DIR}/oauth2/GoogleOAuth2ProviderTest.cpp
	${PROJECT_SOURCE_DIR}/oauth2/OpenIDUserDecoderTest.cpp
	${PROJECT_SOURCE_DIR}/policy/SensorHistoryRulesTest.cpp
	${PROJECT_SOURCE_DIR}/policy/SlidingWindowRateLimiterTest.cpp
	${PROJECT_SOURCE_DIR}/server/CORSFilterTest.cpp
	${PROJECT_SOURCE_DIR}/server/HTTPCustomHeadersFilterTest.cpp
	${PROJECT_SOURCE_DIR}/server/SessionManagerTest.cpp
	${PROJECT_SOURCE_DIR}/server/TestingHTTPServerRequest.cpp
	${PROJECT_SOURCE_DIR}/server/TestingHTTPServerResponse.cpp
	${PROJECT_SOURCE_DIR}/service/AuthServiceImplTest.cpp
	${PROJECT_SOURCE_DIR}/rest/UriPatternTest.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchPlanTest.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchPlanParserTest.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchRefTest.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchTestingPlans.cpp
	${PROJECT_SOURCE_DIR}/util/BitmapsSAXHandlerTest.cpp
	${PROJECT_SOURCE_DIR}/util/DevicesSAXHandlerTest.cpp
	${PROJECT_SOURCE_DIR}/util/SanitizeTest.cpp
	${PROJECT_SOURCE_DIR}/util/SQLPreprocessorTest.cpp
	${PROJECT_SOURCE_DIR}/util/TypesSAXHandlerTest.cpp
	${PROJECT_SOURCE_DIR}/util/UnknownEvaluatorTest.cpp
)

if (LUA_FOUND)
	file(GLOB LUA_TEST_SOURCES
		${PROJECT_SOURCE_DIR}/util/LuaPreprocessorTest.cpp
	)
	list(APPEND TEST_SOURCES ${LUA_TEST_SOURCES})
endif()

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

list(APPEND CORE_LIBS BeeeOnBaseTest)
list(APPEND CORE_LIBS BeeeOnBase)
list(APPEND CORE_LIBS BeeeOnServer)
list(APPEND CORE_LIBS BeeeOnServerDaoPoco)
list(APPEND CORE_LIBS BeeeOnServerSqitch)
list(APPEND CORE_LIBS BeeeOnServerGWS)
list(APPEND CORE_LIBS BeeeOnServerOAuth)

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

if(LUA_FOUND)
	list(APPEND LIBS ${LUA_LIBRARIES})
endif()

if(JANSSON)
	list(APPEND LIBS ${JANSSON})
endif()

if(JWT)
	list(APPEND LIBS ${JWT})
endif()

if(ZEROMQ)
	list(APPEND LIBS ${ZEROMQ})
endif()

TargetLink(test-suite-server "${CORE_LIBS}" "${LIBS}")
