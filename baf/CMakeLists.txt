# Baf CMake
# Marek Beňo 2016

cmake_minimum_required (VERSION 2.8)

project (baf)

set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib/modules/)
message (${CMAKE_MODULE_PATH})
include (CheckCXXCompilerFlag)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g -pedantic -Wextra")

add_subdirectory(tasks/AliveCheck)
add_subdirectory(tasks/FireHazard)
add_subdirectory(tasks/Watchdog)

set (BAF_SOURCES
	src/BaseTaskLoader.cpp
	src/BaseTaskManager.cpp
	src/Calendar.cpp
	src/CombinedTaskInstance.cpp
	src/ConfigParser.cpp
	src/DataMessageParser.cpp
	src/DataMessageRegister.cpp
	src/DatabaseInterface.cpp
	src/GatewayInterface.cpp
	src/GatewayServer.cpp
	src/GatewaySession.cpp
	src/Server.cpp
	src/Session.cpp
	src/Task.cpp
	src/TaskInstance.cpp
	src/TaskLoader.cpp
	src/TaskManager.cpp
	src/TimedTaskInstance.cpp
	src/TriggerTaskInstance.cpp
	src/UserMessageParser.cpp
	src/UserServer.cpp
	src/UserSession.cpp
	src/baf.cpp
	src/pugixml.cpp
	src/unified_logger.cpp
	src/locked_stream.cpp
	)

link_directories("/usr/local/lib64" "/usr/pgsql-9.5/lib")
include_directories("/usr/include/soci" "/usr/pgsql-9.5/include" "../lib" "/usr/local")

find_package(Soci COMPONENTS postgresql REQUIRED)
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
