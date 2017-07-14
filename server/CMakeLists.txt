cmake_minimum_required (VERSION 2.8.11)
project (server C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib/modules)

set (CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -pedantic -Wall -lpthread -ggdb3")
add_definitions(-std=c++11)

if(DEFINED ENV{GIT_ID})
	set (GIT_ID $ENV{GIT_ID})
else()
	exec_program(
		"git"
		${CMAKE_CURRENT_SOURCE_DIR}
		ARGS "describe --always --tags"
		OUTPUT_VARIABLE GIT_ID)
endif()

add_definitions(-DGIT_ID="${GIT_ID}")

add_subdirectory(src)
add_subdirectory(base)
add_subdirectory(test)
add_subdirectory(schema)

install(FILES conf/restui-startup.ini conf/xmlui-startup.ini
	DESTINATION etc/beeeon)
install(FILES conf/restui-api.ini
	DESTINATION etc/beeeon)
install(DIRECTORY conf/config.d
	DESTINATION etc/beeeon)
install(FILES conf/devices.xml conf/types.xml
	DESTINATION etc/beeeon)
install(FILES db/server-queries.ini
	DESTINATION etc/beeeon)
install(DIRECTORY db/
	DESTINATION share/beeeon/db
	FILES_MATCHING
	PATTERN "db/sqitch.conf"
	PATTERN "db/sqitch.plan"
	PATTERN "db/deploy/*.sql"
	PATTERN "db/deploy/*/*.sql"
	PATTERN "db/verify/*.sql"
	PATTERN "db/verify/*/*.sql"
	PATTERN "db/revert/*.sql"
	PATTERN "db/revert/*/*.sql"
	PATTERN "db/tools/*"
	PATTERN "db/test" EXCLUDE
	PATTERN "db/templates" EXCLUDE)
install(DIRECTORY cert/
	DESTINATION share/beeeon/cert
	FILES_MATCHING PATTERN "*.pem")
