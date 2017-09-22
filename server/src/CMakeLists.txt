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
find_library (ICU_I18N icui18n)
find_library (ICU_UC icuuc)
find_library (JWT jwt)
find_library (JANSSON jansson)

add_definitions(-std=c++11)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/jwt/JWToken.cpp
	${PROJECT_SOURCE_DIR}/provider/AuthProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/DeviceInfoProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/RandomProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/OAuth2AuthProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/FacebookAuthProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/GoogleAuthProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/TypeInfoProvider.cpp
	${PROJECT_SOURCE_DIR}/server/AuthRestHandler.cpp
	${PROJECT_SOURCE_DIR}/server/ExpirableSession.cpp
	${PROJECT_SOURCE_DIR}/server/CORSFilter.cpp
	${PROJECT_SOURCE_DIR}/server/HTTPFilter.cpp
	${PROJECT_SOURCE_DIR}/server/HTTPFilterChain.cpp
	${PROJECT_SOURCE_DIR}/server/Session.cpp
	${PROJECT_SOURCE_DIR}/server/SessionCache.cpp
	${PROJECT_SOURCE_DIR}/server/SessionManager.cpp
	${PROJECT_SOURCE_DIR}/server/SessionManagerImpl.cpp
	${PROJECT_SOURCE_DIR}/server/SessionVerifier.cpp
	${PROJECT_SOURCE_DIR}/server/AccessLevel.cpp
	${PROJECT_SOURCE_DIR}/server/XmlRequestHandler.cpp
	${PROJECT_SOURCE_DIR}/server/SocketServer.cpp
	${PROJECT_SOURCE_DIR}/l10n/HTTPLocaleExtractor.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuLocaleImpl.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuLocaleManager.cpp
	${PROJECT_SOURCE_DIR}/l10n/IcuTranslator.cpp
	${PROJECT_SOURCE_DIR}/model/AbstractModule.cpp
	${PROJECT_SOURCE_DIR}/model/Control.cpp
	${PROJECT_SOURCE_DIR}/model/DeviceInfo.cpp
	${PROJECT_SOURCE_DIR}/model/FCMToken.cpp
	${PROJECT_SOURCE_DIR}/model/Identity.cpp
	${PROJECT_SOURCE_DIR}/model/ModuleInfo.cpp
	${PROJECT_SOURCE_DIR}/model/VerifiedIdentity.cpp
	${PROJECT_SOURCE_DIR}/model/RoleInGateway.cpp
	${PROJECT_SOURCE_DIR}/model/LegacyRoleInGateway.cpp
	${PROJECT_SOURCE_DIR}/model/LegacyGateway.cpp
	${PROJECT_SOURCE_DIR}/model/Device.cpp
	${PROJECT_SOURCE_DIR}/model/DeviceProperty.cpp
	${PROJECT_SOURCE_DIR}/model/GatewayStatus.cpp
	${PROJECT_SOURCE_DIR}/model/Location.cpp
	${PROJECT_SOURCE_DIR}/model/Sensor.cpp
	${PROJECT_SOURCE_DIR}/model/ValueAt.cpp
	${PROJECT_SOURCE_DIR}/model/ModuleValue.cpp
	${PROJECT_SOURCE_DIR}/model/TypeInfo.cpp
	${PROJECT_SOURCE_DIR}/model/User.cpp
	${PROJECT_SOURCE_DIR}/dao/ControlDao.cpp
	${PROJECT_SOURCE_DIR}/dao/FCMTokenDao.cpp
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
	${PROJECT_SOURCE_DIR}/dao/poco/PocoSQLControlDao.cpp
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
	${PROJECT_SOURCE_DIR}/service/ControlService.cpp
	${PROJECT_SOURCE_DIR}/service/ControlServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/UserService.cpp
	${PROJECT_SOURCE_DIR}/service/UserServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/AuthService.cpp
	${PROJECT_SOURCE_DIR}/service/AuthServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/DeviceService.cpp
	${PROJECT_SOURCE_DIR}/service/DeviceServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/GWSDeviceService.cpp
	${PROJECT_SOURCE_DIR}/service/GWSDeviceServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/GatewayService.cpp
	${PROJECT_SOURCE_DIR}/service/GatewayServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/GWSGatewayService.cpp
	${PROJECT_SOURCE_DIR}/service/GWSGatewayServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/GWSSensorHistoryService.cpp
	${PROJECT_SOURCE_DIR}/service/GWSSensorHistoryService.cpp
	${PROJECT_SOURCE_DIR}/service/GWSSensorHistoryServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/LocationService.cpp
	${PROJECT_SOURCE_DIR}/service/LocationServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/SensorService.cpp
	${PROJECT_SOURCE_DIR}/service/SensorServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/IdentityService.cpp
	${PROJECT_SOURCE_DIR}/service/IdentityServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/RoleService.cpp
	${PROJECT_SOURCE_DIR}/service/RoleServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/SensorHistoryService.cpp
	${PROJECT_SOURCE_DIR}/service/SensorHistoryServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/service/WorkService.cpp
	${PROJECT_SOURCE_DIR}/service/WorkServiceImpl.cpp
	${PROJECT_SOURCE_DIR}/policy/ControlAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/FCMTokenAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/GatewayAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/IdentityAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/LocationAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/SensorAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/DeviceAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/RoleAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/SensorHistoryAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/WorkAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/DefaultAccessPolicy.cpp
	${PROJECT_SOURCE_DIR}/policy/PolicyContext.cpp
	${PROJECT_SOURCE_DIR}/policy/UserPolicyContext.cpp
	${PROJECT_SOURCE_DIR}/util/DevicesSAXHandler.cpp
	${PROJECT_SOURCE_DIR}/util/Sanitize.cpp
	${PROJECT_SOURCE_DIR}/util/Startup.cpp
	${PROJECT_SOURCE_DIR}/util/TypesSAXHandler.cpp
	${PROJECT_SOURCE_DIR}/dao/poco/PocoDaoManager.cpp
	${PROJECT_SOURCE_DIR}/gws/GatewayConnection.cpp
	${PROJECT_SOURCE_DIR}/gws/GatewayCommunicator.cpp
	${PROJECT_SOURCE_DIR}/gws/GWMessageHandler.cpp
	${PROJECT_SOURCE_DIR}/gws/GWMessageHandlerImpl.cpp
	${PROJECT_SOURCE_DIR}/gws/GWResponseExpectedQueue.cpp
	${PROJECT_SOURCE_DIR}/gws/RPCForwarder.cpp
	${PROJECT_SOURCE_DIR}/gws/WebSocketRequestHandler.cpp
	${PROJECT_SOURCE_DIR}/gws/WebSocketServer.cpp
	${PROJECT_SOURCE_DIR}/rpc/RPCConnector.cpp
	${PROJECT_SOURCE_DIR}/rpc/SocketRPCConnector.cpp
	${PROJECT_SOURCE_DIR}/rpc/GatewayRPC.cpp
	${PROJECT_SOURCE_DIR}/rpc/GatewayRPCResult.cpp
	${PROJECT_SOURCE_DIR}/rpc/AsyncGatewayRPC.cpp
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
	${PROJECT_SOURCE_DIR}/sqitch/PocoSqitchInitializer.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchChange.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchEntry.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchInitializer.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchLine.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchPlan.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchPlanParser.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchPragma.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchRef.cpp
	${PROJECT_SOURCE_DIR}/sqitch/SqitchTag.cpp
	${PROJECT_SOURCE_DIR}/work/TransactionalWorkRunner.cpp
	${PROJECT_SOURCE_DIR}/work/DeviceUnpairWork.cpp
	${PROJECT_SOURCE_DIR}/work/DeviceUnpairWorkExecutor.cpp
	${PROJECT_SOURCE_DIR}/work/GatewayScanWork.cpp
	${PROJECT_SOURCE_DIR}/work/GatewayScanWorkExecutor.cpp
)

if (JWT AND JANSSON)
	file(GLOB JWT_SOURCES
		${PROJECT_SOURCE_DIR}/jwt/JWTDecoder.cpp
		${PROJECT_SOURCE_DIR}/jwt/JWTEncoder.cpp
		${PROJECT_SOURCE_DIR}/server/JWTSessionManager.cpp
	)
	list(APPEND SOURCES ${JWT_SOURCES})
else()
	message(STATUS "JWT support is disabled due to unmet dependencies")
endif()

file(GLOB WORK_SOURCES
	${PROJECT_SOURCE_DIR}/work/AbstractWorkBackup.cpp
	${PROJECT_SOURCE_DIR}/work/AbstractGatewayWork.cpp
	${PROJECT_SOURCE_DIR}/work/BasicProcessor.cpp
	${PROJECT_SOURCE_DIR}/work/BasicQueue.cpp
	${PROJECT_SOURCE_DIR}/work/DefaultWorkFacade.cpp
	${PROJECT_SOURCE_DIR}/work/ExtendedWorkExecutor.cpp
	${PROJECT_SOURCE_DIR}/work/GenericWorkRunner.cpp
	${PROJECT_SOURCE_DIR}/work/Work.cpp
	${PROJECT_SOURCE_DIR}/work/WorkBackup.cpp
	${PROJECT_SOURCE_DIR}/work/WorkContent.cpp
	${PROJECT_SOURCE_DIR}/work/WorkExecutor.cpp
	${PROJECT_SOURCE_DIR}/work/WorkFacade.cpp
	${PROJECT_SOURCE_DIR}/work/WorkFactory.cpp
	${PROJECT_SOURCE_DIR}/work/WorkLockManager.cpp
	${PROJECT_SOURCE_DIR}/work/WorkRunner.cpp
	${PROJECT_SOURCE_DIR}/work/WorkScheduler.cpp
	${PROJECT_SOURCE_DIR}/work/WorkSuspendThrowable.cpp
)

file(GLOB XMLUI_SOURCES
	xmlui/Serializing.cpp
	xmlui/XmlValueConsumer.cpp
	xmlui/AbstractXmlHandler.cpp
	xmlui/AnyXmlHandler.cpp
	xmlui/AuthXmlHandler.cpp
	xmlui/SessionXmlHandler.cpp
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
	dao/poco/PocoSQLiteConnectorLoader.cpp
)

file(GLOB RESTUI_SOURCES
	restui/Serializing.cpp
	restui/JSONDeviceDeserializer.cpp
	restui/JSONGatewayDeserializer.cpp
	restui/JSONLocationDeserializer.cpp
	restui/JSONRoleInGatewayDeserializer.cpp
	restui/ControlRestHandler.cpp
	restui/DeviceRestHandler.cpp
	restui/LocationRestHandler.cpp
	restui/ProfileRestHandler.cpp
	restui/RestValueConsumer.cpp
	restui/RoleRestHandler.cpp
	restui/SensorRestHandler.cpp
	restui/TypeRestHandler.cpp
	restui/GatewayRestHandler.cpp
	restui/WorkRestHandler.cpp
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
add_library(BeeeOnWork SHARED ${WORK_SOURCES})

file(GLOB MAIN_CPP main.cpp)
configure_file(${MAIN_CPP} ${CMAKE_CURRENT_BINARY_DIR}/main.cpp)

add_executable(beeeon-server
	${CMAKE_CURRENT_BINARY_DIR}/main.cpp
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
	${ICU_I18N}
	${ICU_UC}
)

if(JANSSON)
	list(APPEND LIBS ${JANSSON})
else()
	message(STATUS "library jansson was not found, ignoring...")
endif()

if(JWT)
	list(APPEND LIBS ${JWT})
else()
	message(STATUS "library jwt was not found, ignoring...")
endif()

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

install(TARGETS beeeon-server BeeeOnServer BeeeOnWork
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION "${LIB_INSTALL_DIR}"
	ARCHIVE DESTINATION "${AR_INSTALL_DIR}"
)
