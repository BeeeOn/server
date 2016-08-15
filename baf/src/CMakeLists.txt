# Baf CMake
# Marek Beňo 2016

cmake_minimum_required (VERSION 2.8)

project (baf)

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/modules/)
message (${CMAKE_MODULE_PATH})
include (CheckCXXCompilerFlag)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -pedantic -Wextra")

set (BAF_SOURCES
	app/baf.cpp
	app/ConfigParser.cpp
	core/BaseTaskLoader.cpp
	core/BaseTaskManager.cpp
	core/Calendar.cpp
	core/DataMessageRegister.cpp
	core/TaskLoader.cpp
	core/TaskManager.cpp
	model/CombinedTaskInstance.cpp
	model/Task.cpp
	model/TaskInstance.cpp
	model/TimedTaskInstance.cpp
	model/TriggerTaskInstance.cpp
	parse/DataMessageParser.cpp
	parse/UserMessageParser.cpp
	server/DatabaseInterface.cpp
	server/GatewayInterface.cpp
	server/GatewayServer.cpp
	server/GatewaySession.cpp
	server/Server.cpp
	server/Session.cpp
	server/UserServer.cpp
	server/UserSession.cpp
	pugixml.cpp
	unified_logger.cpp
	locked_stream.cpp
	)

include_directories(
	${PROJECT_SOURCE_DIR}
	${SOCI_INCLUDE_DIRS}
	"../../lib"
	"/usr/local"
	"/usr/include/soci"
)

add_subdirectory(tasks)

find_package(Soci COMPONENTS postgresql REQUIRED)
message(${SOCI_LIBRARY})
find_package (Threads)
find_library (POCO_FOUNDATION PocoFoundation)
find_library (POCO_UTIL PocoUtil)

add_executable (${PROJECT_NAME} ${BAF_SOURCES})

target_link_libraries (
	${PROJECT_NAME}
	${CMAKE_THREAD_LIBS_INIT}
	${CMAKE_DL_LIBS}
	${SOCI_LIBRARY}
	${SOCI_postgresql_PLUGIN}
	${POCO_FOUNDATION}
	${POCO_UTIL}
	)
