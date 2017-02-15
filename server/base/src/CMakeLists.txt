cmake_minimum_required (VERSION 2.8.11)
project (base-library CXX)

find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_SSL PocoNetSSL)
find_library (POCO_CRYPTO PocoCrypto)
find_library (POCO_UTIL PocoUtil)
find_library (POCO_NET PocoNet)
find_library (POCO_XML PocoXML)

add_definitions(-std=c++11)

file(GLOB SOURCES
	${PROJECT_SOURCE_DIR}/di/DependencyInjector.cpp
	${PROJECT_SOURCE_DIR}/di/DIWrapper.cpp
	${PROJECT_SOURCE_DIR}/loop/StoppableRunnable.cpp
	${PROJECT_SOURCE_DIR}/loop/StoppableLoop.cpp
	${PROJECT_SOURCE_DIR}/loop/LoopRunner.cpp
	${PROJECT_SOURCE_DIR}/model/GatewayID.cpp
	${PROJECT_SOURCE_DIR}/model/DeviceID.cpp
	${PROJECT_SOURCE_DIR}/model/TypeInfo.cpp
	${PROJECT_SOURCE_DIR}/model/ModuleInfo.cpp
	${PROJECT_SOURCE_DIR}/model/DeviceInfo.cpp
	${PROJECT_SOURCE_DIR}/provider/TypeInfoProvider.cpp
	${PROJECT_SOURCE_DIR}/provider/DeviceInfoProvider.cpp
	${PROJECT_SOURCE_DIR}/ssl/X509Fingerprint.cpp
	${PROJECT_SOURCE_DIR}/ssl/RejectCertificateHandler.cpp
	${PROJECT_SOURCE_DIR}/ssl/SSLFacility.cpp
	${PROJECT_SOURCE_DIR}/ssl/SSLClient.cpp
	${PROJECT_SOURCE_DIR}/ssl/SSLServer.cpp
	${PROJECT_SOURCE_DIR}/util/DAMM.cpp
	${PROJECT_SOURCE_DIR}/util/SecureXmlParser.cpp
	${PROJECT_SOURCE_DIR}/util/SAXHelper.cpp
	${PROJECT_SOURCE_DIR}/util/TypesSAXHandler.cpp
	${PROJECT_SOURCE_DIR}/util/DevicesSAXHandler.cpp
	${PROJECT_SOURCE_DIR}/util/Template.cpp
	${PROJECT_SOURCE_DIR}/util/Occasionally.cpp
	${PROJECT_SOURCE_DIR}/util/Loggable.cpp
	${PROJECT_SOURCE_DIR}/util/PosixSignal.cpp
	${PROJECT_SOURCE_DIR}/util/TimeInterval.cpp
	${PROJECT_SOURCE_DIR}/util/ValueGenerator.cpp
)

include_directories(
	${PROJECT_SOURCE_DIR}
)

add_library(BeeeOnBase SHARED ${SOURCES})

install(TARGETS BeeeOnBase
	LIBRARY DESTINATION usr/lib
	ARCHIVE DESTINATION usr/lib
)
