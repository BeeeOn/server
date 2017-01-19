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
	${PROJECT_SOURCE_DIR}/provider/TypeInfoProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/DeviceInfoProvider.cpp
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
	${PROJECT_SOURCE_DIR}/model/DeviceID.cpp
	${PROJECT_SOURCE_DIR}/model/Identity.cpp
	${PROJECT_SOURCE_DIR}/model/VerifiedIdentity.cpp
	${PROJECT_SOURCE_DIR}/model/RoleInGateway.cpp
	${PROJECT_SOURCE_DIR}/model/LegacyGateway.cpp
	${PROJECT_SOURCE_DIR}/model/Device.cpp
	${PROJECT_SOURCE_DIR}/model/Location.cpp
	${PROJECT_SOURCE_DIR}/model/TypeInfo.cpp
	${PROJECT_SOURCE_DIR}/model/ModuleInfo.cpp
	${PROJECT_SOURCE_DIR}/model/DeviceInfo.cpp
	${PROJECT_SOURCE_DIR}/dao/UserDao.cpp
	${PROJECT_SOURCE_DIR}/dao/DeviceDao.cpp
	${PROJECT_SOURCE_DIR}/dao/GatewayDao.cpp
	${PROJECT_SOURCE_DIR}/dao/LocationDao.cpp
	${PROJECT_SOURCE_DIR}/dao/IdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/VerifiedIdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/RoleInGatewayDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoTransactionImpl.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoTransactionManager.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoAbstractDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLUserDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLVerifiedIdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLIdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLRoleInGatewayDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLGatewayDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLLocationDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLDeviceDao.cpp
	${PROJECT_SOURCE_DIR}/dao/ConnectorLoader.cpp
	${PROJECT_SOURCE_DIR}/dao/SQLLoader.cpp
	${PROJECT_SOURCE_DIR}/dao/SQLQuery.cpp
	${PROJECT_SOURCE_DIR}/dao/TransactionManager.cpp
	${PROJECT_SOURCE_DIR}/dao/ThreadLocalTransactionManager.cpp
	${PROJECT_SOURCE_DIR}/dao/NullTransactionManager.cpp
	${PROJECT_SOURCE_DIR}/dao/NoTransactionManager.cpp
	${PROJECT_SOURCE_DIR}/dao/Transactional.cpp
	${PROJECT_SOURCE_DIR}/service/UserService.cpp
	${PROJECT_SOURCE_DIR}/service/AuthService.cpp
	${PROJECT_SOURCE_DIR}/service/DeviceService.cpp
	${PROJECT_SOURCE_DIR}/service/GatewayService.cpp
	${PROJECT_SOURCE_DIR}/service/LocationService.cpp
	${PROJECT_SOURCE_DIR}/service/IdentityService.cpp
	${PROJECT_SOURCE_DIR}/service/RoleService.cpp
	${PROJECT_SOURCE_DIR}/service/JSONGatewayDeserializer.cpp
	${PROJECT_SOURCE_DIR}/service/JSONLocationDeserializer.cpp
	${PROJECT_SOURCE_DIR}/policy/GatewayAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/LocationAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/DeviceAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/RoleAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/DefaultAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/util/DAMM.cpp
	${PROJECT_SOURCE_DIR}/util/SecureXmlParser.cpp
	${PROJECT_SOURCE_DIR}/util/SAXHelper.cpp
	${PROJECT_SOURCE_DIR}/util/TypesSAXHandler.cpp
	${PROJECT_SOURCE_DIR}/util/DevicesSAXHandler.cpp
	${PROJECT_SOURCE_DIR}/util/Template.cpp
	${PROJECT_SOURCE_DIR}/util/Occasionally.cpp
	${PROJECT_SOURCE_DIR}/util/Startup.cpp
	${PROJECT_SOURCE_DIR}/util/Loggable.cpp
	${PROJECT_SOURCE_DIR}/util/PosixSignal.cpp
	${PROJECT_SOURCE_DIR}/ssl/X509Fingerprint.cpp
	${PROJECT_SOURCE_DIR}/ssl/RejectCertificateHandler.cpp
	${PROJECT_SOURCE_DIR}/ssl/SSLFacility.cpp
	${PROJECT_SOURCE_DIR}/ssl/SSLClient.cpp
	${PROJECT_SOURCE_DIR}/ssl/SSLServer.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoDaoManager.cpp
	${PROJECT_SOURCE_DIR}/rpc/GatewayRPC.cpp
	${PROJECT_SOURCE_DIR}/rpc/FakeGatewayRPC.cpp
	${PROJECT_SOURCE_DIR}/notification/NotificationDispatcher.cpp
	${PROJECT_SOURCE_DIR}/notification/FirstLoginNotification.cpp
	${PROJECT_SOURCE_DIR}/notification/InvitedNotification.cpp
	${PROJECT_SOURCE_DIR}/notification/NotificationObserver.cpp
	${PROJECT_SOURCE_DIR}/notification/LoggingObserver.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}
)

add_library(BeeeOnServer ${SOURCES})

add_executable(ui-server
	ui/main.cpp
	ui/UIServerModule.cpp
	ui/Serializing.cpp
	ui/UIMockInit.cpp
	dao/poco/PocoSQLiteConnectorLoader.cpp
)

add_executable(xmlui-server
	xmlui/main.cpp
	xmlui/Serializing.cpp
	xmlui/AbstractXmlHandler.cpp
	xmlui/AnyXmlHandler.cpp
	xmlui/AuthXmlHandler.cpp
	xmlui/XmlGatewayDeserializer.cpp
	xmlui/XmlLocationDeserializer.cpp
	xmlui/XmlDeviceDeserializer.cpp
	xmlui/XmlRoleInGatewayDeserializer.cpp
	xmlui/GatewayXmlHandler.cpp
	xmlui/LocationXmlHandler.cpp
	xmlui/ProfileXmlHandler.cpp
	xmlui/DeviceXmlHandler.cpp
	ui/UIMockInit.cpp
	dao/poco/PocoSQLiteConnectorLoader.cpp
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
