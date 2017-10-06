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

exec_program(
	"genrb"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS
	-d res/beeeon_server
	res/beeeon_server/res_index.txt
	res/beeeon_server/root.txt
	res/beeeon_server/en.txt
	res/beeeon_server/cs.txt
)

# When installing into /usr, use /etc for configuration
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr")
set(SYSCONF_INSTALL_DIR "/etc")
else()
set(SYSCONF_INSTALL_DIR "etc")
endif()

add_subdirectory(src)
add_subdirectory(base)
add_subdirectory(test)

find_package(Doxygen)

if(DOXYGEN_FOUND)
	set(DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile.in)
	set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT})
	add_custom_target(doc
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		COMMENT "Generating API documentation with Doxygen"
		VERBATIM)
endif()

install(FILES conf/restui-startup.ini conf/xmlui-startup.ini
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(FILES conf/restui-api.ini
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(DIRECTORY conf/config.d
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(FILES conf/devices.xml conf/types.xml
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
install(FILES db/server-queries.ini
	DESTINATION ${SYSCONF_INSTALL_DIR}/beeeon/server)
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
install(FILES dtd/types.dtd
	DESTINATION share/beeeon/dtd)
