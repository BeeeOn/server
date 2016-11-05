cmake_minimum_required (VERSION 2.8.11)
project (ui-server C CXX)

find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_SSL PocoNetSSL)
find_library (POCO_CRYPTO PocoCrypto)
find_library (POCO_UTIL PocoUtil)
find_library (POCO_NET PocoNet)
find_library (POCO_JSON PocoJSON)
find_library (POCO_XML PocoXML)
find_library (POCO_DATA PocoData)
find_library (POCO_SQLITE PocoDataSQLite)

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -DBEEEON_SELECT_POCO")
add_definitions(-std=c++11)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/mongoose/MongooseResponse.cpp
	${PROJECT_SOURCE_DIR}/di/InjectorTarget.cpp
	${PROJECT_SOURCE_DIR}/di/DependencyInjector.cpp
	${PROJECT_SOURCE_DIR}/provider/AuthProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/RandomProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/OAuth2AuthProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/GoogleAuthProvider.cpp
	${PROJECT_SOURCE_DIR}/server/MongooseServer.cpp
	${PROJECT_SOURCE_DIR}/server/SessionCache.cpp
	${PROJECT_SOURCE_DIR}/server/SessionManager.cpp
	${PROJECT_SOURCE_DIR}/server/SessionVerifier.cpp
	${PROJECT_SOURCE_DIR}/server/RestHandler.cpp
	${PROJECT_SOURCE_DIR}/server/RestAuthHandler.cpp
	${PROJECT_SOURCE_DIR}/server/AccessLevel.cpp
	${PROJECT_SOURCE_DIR}/server/XmlRequestHandler.cpp
	${PROJECT_SOURCE_DIR}/server/SocketServer.cpp
	${PROJECT_SOURCE_DIR}/model/GatewayID.cpp
	${PROJECT_SOURCE_DIR}/model/JSONSerializer.cpp
	${PROJECT_SOURCE_DIR}/model/Identity.cpp
	${PROJECT_SOURCE_DIR}/model/VerifiedIdentity.cpp
	${PROJECT_SOURCE_DIR}/model/RoleInPlace.cpp
	${PROJECT_SOURCE_DIR}/dao/SQLDialect.cpp
	${PROJECT_SOURCE_DIR}/dao/PlaceDao.cpp
	${PROJECT_SOURCE_DIR}/dao/UserDao.cpp
	${PROJECT_SOURCE_DIR}/dao/DeviceDao.cpp
	${PROJECT_SOURCE_DIR}/dao/GatewayDao.cpp
	${PROJECT_SOURCE_DIR}/dao/LocationDao.cpp
	${PROJECT_SOURCE_DIR}/dao/IdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/VerifiedIdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/RoleInPlaceDao.cpp
	${PROJECT_SOURCE_DIR}/dao/PocoAbstractDao.cpp
	${PROJECT_SOURCE_DIR}/dao/PocoSQLPlaceDao.cpp
	${PROJECT_SOURCE_DIR}/dao/PocoSQLUserDao.cpp
	${PROJECT_SOURCE_DIR}/dao/PocoSQLVerifiedIdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/PocoSQLIdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/PocoSQLRoleInPlaceDao.cpp
	${PROJECT_SOURCE_DIR}/dao/PocoSQLGatewayDao.cpp
	${PROJECT_SOURCE_DIR}/dao/PocoSQLLocationDao.cpp
	${PROJECT_SOURCE_DIR}/service/PlaceService.cpp
	${PROJECT_SOURCE_DIR}/service/UserService.cpp
	${PROJECT_SOURCE_DIR}/service/AuthService.cpp
	${PROJECT_SOURCE_DIR}/service/DeviceService.cpp
	${PROJECT_SOURCE_DIR}/service/GatewayService.cpp
	${PROJECT_SOURCE_DIR}/service/LocationService.cpp
	${PROJECT_SOURCE_DIR}/service/NotificationService.cpp
	${PROJECT_SOURCE_DIR}/service/IdentityService.cpp
	${PROJECT_SOURCE_DIR}/service/JSONPlaceDeserializer.cpp
	${PROJECT_SOURCE_DIR}/service/JSONGatewayDeserializer.cpp
	${PROJECT_SOURCE_DIR}/service/JSONLocationDeserializer.cpp
	${PROJECT_SOURCE_DIR}/policy/PlaceAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/GatewayAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/LocationAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/DefaultAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/util/DAMM.cpp
	${PROJECT_SOURCE_DIR}/util/SecureXmlParser.cpp
	${PROJECT_SOURCE_DIR}/util/Template.cpp
	${PROJECT_SOURCE_DIR}/util/Occasionally.cpp
	${PROJECT_SOURCE_DIR}/util/SSLFacility.cpp
	${PROJECT_SOURCE_DIR}/util/SSLClient.cpp
	${PROJECT_SOURCE_DIR}/util/SSLServer.cpp
	${PROJECT_SOURCE_DIR}/dao/PocoDaoManager.cpp
	${PROJECT_SOURCE_DIR}/rpc/GatewayRPC.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}
)

add_library(BeeeOnServer ${SOURCES})

add_executable(ui-server
	ui/main.cpp
	ui/UIServerModule.cpp
	ui/Serializing.cpp
	ui/PlaceHandler.cpp
	ui/GatewayHandler.cpp
	ui/LocationHandler.cpp
	ui/UIMockInit.cpp
)

add_executable(xmlui-server
	xmlui/main.cpp
	xmlui/Serializing.cpp
	xmlui/AbstractXmlHandler.cpp
	xmlui/AnyXmlHandler.cpp
	xmlui/AuthXmlHandler.cpp
	xmlui/XmlGatewayDeserializer.cpp
	xmlui/XmlLocationDeserializer.cpp
	xmlui/GatewayXmlHandler.cpp
	xmlui/LocationXmlHandler.cpp
	xmlui/ProfileXmlHandler.cpp
	ui/UIMockInit.cpp
)

set(LIBS
	${POCO_FOUNDATION}
	${POCO_SSL}
	${POCO_CRYPTO}
	${POCO_UTIL}
	${POCO_NET}
	${POCO_JSON}
	${POCO_XML}
	${POCO_DATA}
	${POCO_SQLITE}
	mongoose
)

# Apple's linker doesn't support --whole-archive. Instead it uses -all_load.
if (APPLE)
	target_link_libraries(ui-server
		-Wl,-all_load
		BeeeOnServer
		${LIBS}
	)
	target_link_libraries(xmlui-server
		-Wl,-all_load
		BeeeOnServer
		${LIBS}
	)
# The -Wl,--whole-archive is necessary for all libraries defining
# BEEEON_OBJECTs (see di/InjectorTarget.h).
else()
	target_link_libraries(ui-server
		-Wl,--whole-archive
		BeeeOnServer
		-Wl,--no-whole-archive
		${LIBS}
	)
	target_link_libraries(xmlui-server
		-Wl,--whole-archive
		BeeeOnServer
		-Wl,--no-whole-archive
		${LIBS}
	)
endif()
