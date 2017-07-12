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
find_library (POCO_ODBC PocoDataODBC)

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -DBEEEON_SELECT_POCO")
add_definitions(-std=c++11)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/provider/AuthProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/RandomProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/OAuth2AuthProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/FacebookAuthProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/GoogleAuthProvider.cpp
	${PROJECT_SOURCE_DIR}/server/AuthRestHandler.cpp
	${PROJECT_SOURCE_DIR}/server/SessionCache.cpp
	${PROJECT_SOURCE_DIR}/server/SessionManager.cpp
	${PROJECT_SOURCE_DIR}/server/SessionVerifier.cpp
	${PROJECT_SOURCE_DIR}/server/AccessLevel.cpp
	${PROJECT_SOURCE_DIR}/server/XmlRequestHandler.cpp
	${PROJECT_SOURCE_DIR}/server/SocketServer.cpp
	${PROJECT_SOURCE_DIR}/model/Identity.cpp
	${PROJECT_SOURCE_DIR}/model/VerifiedIdentity.cpp
	${PROJECT_SOURCE_DIR}/model/RoleInGateway.cpp
	${PROJECT_SOURCE_DIR}/model/LegacyRoleInGateway.cpp
	${PROJECT_SOURCE_DIR}/model/LegacyGateway.cpp
	${PROJECT_SOURCE_DIR}/model/Device.cpp
	${PROJECT_SOURCE_DIR}/model/DeviceProperty.cpp
	${PROJECT_SOURCE_DIR}/model/Location.cpp
	${PROJECT_SOURCE_DIR}/model/ValueAt.cpp
	${PROJECT_SOURCE_DIR}/model/ModuleValue.cpp
	${PROJECT_SOURCE_DIR}/dao/UserDao.cpp
	${PROJECT_SOURCE_DIR}/dao/DeviceDao.cpp
	${PROJECT_SOURCE_DIR}/dao/DevicePropertyDao.cpp
	${PROJECT_SOURCE_DIR}/dao/GatewayDao.cpp
	${PROJECT_SOURCE_DIR}/dao/LocationDao.cpp
	${PROJECT_SOURCE_DIR}/dao/IdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/VerifiedIdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/RoleInGatewayDao.cpp
	${PROJECT_SOURCE_DIR}/dao/SensorHistoryDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoTransactionImpl.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoTransactionFactory.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoAbstractDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoDBInitializer.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSimpleDBInitializer.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLUserDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLVerifiedIdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLIdentityDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLRoleInGatewayDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLGatewayDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLGatewayStatusDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLLocationDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLDeviceDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLDevicePropertyDao.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLSensorHistoryDao.cpp
	${PROJECT_SOURCE_DIR}/dao/ConnectorLoader.cpp
	${PROJECT_SOURCE_DIR}/dao/SQLLoader.cpp
	${PROJECT_SOURCE_DIR}/dao/SQLQuery.cpp
	${PROJECT_SOURCE_DIR}/service/UserService.cpp
	${PROJECT_SOURCE_DIR}/service/AuthService.cpp
	${PROJECT_SOURCE_DIR}/service/DeviceService.cpp
	${PROJECT_SOURCE_DIR}/service/GatewayService.cpp
	${PROJECT_SOURCE_DIR}/service/LocationService.cpp
	${PROJECT_SOURCE_DIR}/service/IdentityService.cpp
	${PROJECT_SOURCE_DIR}/service/RoleService.cpp
	${PROJECT_SOURCE_DIR}/service/SensorHistoryService.cpp
	${PROJECT_SOURCE_DIR}/service/JSONGatewayDeserializer.cpp
	${PROJECT_SOURCE_DIR}/service/JSONLocationDeserializer.cpp
	${PROJECT_SOURCE_DIR}/policy/GatewayAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/LocationAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/DeviceAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/RoleAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/SensorHistoryAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/DefaultAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/util/Startup.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoDaoManager.cpp
	${PROJECT_SOURCE_DIR}/rpc/RPCConnector.cpp
	${PROJECT_SOURCE_DIR}/rpc/SocketRPCConnector.cpp
	${PROJECT_SOURCE_DIR}/rpc/GatewayRPC.cpp
	${PROJECT_SOURCE_DIR}/rpc/FakeGatewayRPC.cpp
	${PROJECT_SOURCE_DIR}/rpc/LegacyGatewayRPC.cpp
	${PROJECT_SOURCE_DIR}/notification/NotificationDispatcher.cpp
	${PROJECT_SOURCE_DIR}/notification/FirstLoginNotification.cpp
	${PROJECT_SOURCE_DIR}/notification/InvitedNotification.cpp
	${PROJECT_SOURCE_DIR}/notification/NotificationObserver.cpp
	${PROJECT_SOURCE_DIR}/notification/LoggingObserver.cpp
	${PROJECT_SOURCE_DIR}/rest/BuiltinRestHandler.cpp
	${PROJECT_SOURCE_DIR}/rest/JSONRestHandler.cpp
	${PROJECT_SOURCE_DIR}/rest/MappedRestAction.cpp
	${PROJECT_SOURCE_DIR}/rest/PocoContext.cpp
	${PROJECT_SOURCE_DIR}/rest/PocoRestRequestHandler.cpp
	${PROJECT_SOURCE_DIR}/rest/PocoRestServer.cpp
	${PROJECT_SOURCE_DIR}/rest/RestAction.cpp
	${PROJECT_SOURCE_DIR}/rest/RestFlow.cpp
	${PROJECT_SOURCE_DIR}/rest/RestHandler.cpp
	${PROJECT_SOURCE_DIR}/rest/RestLinker.cpp
	${PROJECT_SOURCE_DIR}/rest/RestRouter.cpp
	${PROJECT_SOURCE_DIR}/rest/UriPattern.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchChange.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchEntry.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchLine.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchPlan.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchPlanParser.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchPragma.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchRef.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchTag.cpp
	${PROJECT_SOURCE_DIR}/work/TransactionalWorkRunner.cpp
	${PROJECT_SOURCE_DIR}/work/DeviceUnpairWork.cpp
	${PROJECT_SOURCE_DIR}/work/DeviceUnpairWorkExecutor.cpp
)

file(GLOB XMLUI_SOURCES
	xmlui/Serializing.cpp
	xmlui/XmlValueConsumer.cpp
	xmlui/AbstractXmlHandler.cpp
	xmlui/AnyXmlHandler.cpp
	xmlui/AuthXmlHandler.cpp
	xmlui/XmlGatewayDeserializer.cpp
	xmlui/XmlLocationDeserializer.cpp
	xmlui/XmlDeviceDeserializer.cpp
	xmlui/XmlDevicePropertyDeserializer.cpp
	xmlui/XmlRoleInGatewayDeserializer.cpp
	xmlui/GatewayXmlHandler.cpp
	xmlui/LocationXmlHandler.cpp
	xmlui/ProfileXmlHandler.cpp
	xmlui/DeviceXmlHandler.cpp
	xmlui/SensorXmlHandler.cpp
	xmlui/RoleXmlHandler.cpp
	ui/UIMockInit.cpp
	dao/poco/PocoSQLiteConnectorLoader.cpp
)

file(GLOB RESTUI_SOURCES
	restui/Serializing.cpp
	restui/JSONGatewayDeserializer.cpp
	restui/GatewayRestHandler.cpp
	ui/UIMockInit.cpp
	dao/poco/PocoSQLiteConnectorLoader.cpp
)

if (POCO_ODBC)
	list(APPEND XMLUI_SOURCES  dao/poco/PocoODBCConnectorLoader.cpp)
	list(APPEND RESTUI_SOURCES dao/poco/PocoODBCConnectorLoader.cpp)
endif()

include_directories(
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/base/src
)

add_library(BeeeOnServer SHARED ${SOURCES})

add_executable(beeeon-server
	main.cpp
	${XMLUI_SOURCES}
	${RESTUI_SOURCES}
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
)

# Apple's linker doesn't support --whole-archive. Instead it uses -all_load.
if (APPLE)
	target_link_libraries(beeeon-server
		-Wl,-all_load
		BeeeOnBase
		BeeeOnWork
		BeeeOnTransaction
		BeeeOnServer
		${LIBS}
	)
# The -Wl,--whole-archive is necessary for all libraries defining
# BEEEON_OBJECTs (see di/Injectable.h).
else()
	target_link_libraries(beeeon-server
		-Wl,--whole-archive
		BeeeOnBase
		BeeeOnWork
		BeeeOnTransaction
		BeeeOnServer
		-Wl,--no-whole-archive
		${LIBS}
	)
endif()

if (NOT POCO_ODBC)
	message(WARNING "ODBC is NOT compiled in")
else()
	target_link_libraries(beeeon-server ${POCO_ODBC})
endif()

install(TARGETS beeeon-server BeeeOnServer
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)
